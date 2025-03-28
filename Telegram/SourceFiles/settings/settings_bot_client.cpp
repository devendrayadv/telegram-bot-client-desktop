#include "settings/settings_bot_client.h"

#include "settings/settings_common.h"
#include "ui/widgets/checkbox.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "ui/widgets/input_fields.h"
#include "ui/wrap/vertical_layout.h"
#include "ui/wrap/slide_wrap.h"
#include "ui/wrap/padding_wrap.h"
#include "ui/toast/toast.h"
#include "lang/lang_keys.h"
#include "core/application.h"
#include "core/core_settings.h"
#include "window/window_session_controller.h"
#include "main/main_session.h"
#include "base/platform/base_platform_info.h"
#include "base/event_filter.h"
#include "chat/chat_bot_logger.h"
#include "styles/style_settings.h"
#include "styles/style_layers.h"

namespace Settings {

BotClient::BotClient(
    QWidget *parent,
    not_null<Window::SessionController*> controller)
: Section(parent)
, _controller(controller)
, _session(&controller->session()) {
    setupContent();
}

void BotClient::setupContent() {
    const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
    
    Ui::AddSkip(content);
    Ui::AddSubsectionTitle(content, tr::lng_settings_bot_client());
    
    setupLoggingOptions();
    setupAutoStartOptions();
    setupPrivacyOptions();
    setupUpdateOptions();
    
    Ui::AddSkip(content);
    
    Ui::ResizeFitChild(this, content);
}

void BotClient::setupLoggingOptions() {
    const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
    
    Ui::AddDividerText(content, tr::lng_settings_bot_logging_about());
    
    const auto enableLogging = content->add(
        object_ptr<Ui::Checkbox>(
            content,
            tr::lng_settings_bot_enable_logging(),
            Core::App().settings().botLoggingEnabled(),
            st::settingsCheckbox),
        st::settingsSectionPadding);
    
    const auto logLevelWrap = content->add(
        object_ptr<Ui::SlideWrap<Ui::VerticalLayout>>(
            content,
            object_ptr<Ui::VerticalLayout>(content)),
        st::settingsSectionPadding);
    
    const auto logLevel = logLevelWrap->entity();
    
    Ui::AddSkip(logLevel);
    
    const auto logLevelGroup = std::make_shared<Ui::RadioenumGroup<BotLogger::LogLevel>>(
        Core::App().settings().botLogLevel());
    
    const auto addOption = [&](
            rpl::producer<QString> text,
            BotLogger::LogLevel value) {
        logLevel->add(
            object_ptr<Ui::Radioenum<BotLogger::LogLevel>>(
                logLevel,
                logLevelGroup,
                value,
                std::move(text)),
            st::settingsSectionPadding);
    };
    
    addOption(tr::lng_settings_bot_log_debug(), BotLogger::LogLevel::Debug);
    addOption(tr::lng_settings_bot_log_info(), BotLogger::LogLevel::Info);
    addOption(tr::lng_settings_bot_log_warning(), BotLogger::LogLevel::Warning);
    addOption(tr::lng_settings_bot_log_error(), BotLogger::LogLevel::Error);
    addOption(tr::lng_settings_bot_log_critical(), BotLogger::LogLevel::Critical);
    
    const auto logPathWrap = content->add(
        object_ptr<Ui::SlideWrap<Ui::VerticalLayout>>(
            content,
            object_ptr<Ui::VerticalLayout>(content)),
        st::settingsSectionPadding);
    
    const auto logPath = logPathWrap->entity();
    
    Ui::AddSkip(logPath);
    
    const auto currentPath = Core::App().settings().botLogPath().isEmpty()
        ? BotLogger::logFilePath()
        : Core::App().settings().botLogPath();
    
    auto pathField = object_ptr<Ui::InputField>(
        logPath,
        st::settingsLogPathInput,
        rpl::single(QString()),
        currentPath);
    
    const auto pathLabel = Ui::CreateChild<Ui::FlatLabel>(
        logPath,
        tr::lng_settings_bot_log_path(),
        st::settingsLabel);
    
    const auto browseButton = Ui::CreateChild<Ui::RoundButton>(
        logPath,
        tr::lng_settings_bot_log_browse(),
        st::settingsBrowseButton);
    
    logPath->add(
        object_ptr<Ui::FixedHeightWidget>(
            logPath,
            st::settingsLogPathInputField.heightMin),
        st::settingsSectionPadding);
    
    pathLabel->moveToLeft(
        st::settingsSectionPadding.left(),
        st::settingsSectionPadding.top());
    
    pathField->moveToLeft(
        st::settingsSectionPadding.left(),
        pathLabel->y() + pathLabel->height() + st::settingsSectionSkip);
    
    browseButton->moveToLeft(
        pathField->x() + pathField->width() + st::settingsSectionSkip,
        pathField->y());
    
    logPath->add(std::move(pathField), st::settingsSectionPadding);
    
    logPathWrap->toggleOn(enableLogging->checkedValue());
    logLevelWrap->toggleOn(enableLogging->checkedValue());
    
    enableLogging->checkedChanges(
    ) | rpl::start_with_next([=](bool checked) {
        Core::App().settings().setBotLoggingEnabled(checked);
        BotLogger::setEnabled(checked);
        logPathWrap->toggle(checked, anim::type::normal);
        logLevelWrap->toggle(checked, anim::type::normal);
    }, enableLogging->lifetime());
    
    logLevelGroup->setChangedCallback([=](BotLogger::LogLevel level) {
        Core::App().settings().setBotLogLevel(level);
        BotLogger::setMinLevel(level);
    });
    
    browseButton->setClickedCallback([=] {
        // TODO: Add file dialog to select log path
        const auto newPath = QFileDialog::getSaveFileName(
            this,
            tr::lng_settings_bot_log_choose(tr::now),
            QDir::homePath(),
            "Log files (*.log)");
        
        if (!newPath.isEmpty()) {
            pathField->setText(newPath);
            Core::App().settings().setBotLogPath(newPath);
            BotLogger::setLogFile(newPath);
        }
    });
    
    content->resizeToWidth(st::settingsWidth);
    
    Ui::AddSkip(content);
}

void BotClient::setupAutoStartOptions() {
    const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
    
    Ui::AddDividerText(content, tr::lng_settings_bot_autostart_about());
    
    const auto autoStart = content->add(
        object_ptr<Ui::Checkbox>(
            content,
            tr::lng_settings_bot_autostart(),
            Core::App().settings().botAutoStart(),
            st::settingsCheckbox),
        st::settingsSectionPadding);
    
    const auto minimizeToTray = content->add(
        object_ptr<Ui::Checkbox>(
            content,
            tr::lng_settings_bot_minimize_tray(),
            Core::App().settings().botMinimizeToTray(),
            st::settingsCheckbox),
        st::settingsSectionPadding);
    
    const auto startMinimized = content->add(
        object_ptr<Ui::Checkbox>(
            content,
            tr::lng_settings_bot_start_minimized(),
            Core::App().settings().botStartMinimized(),
            st::settingsCheckbox),
        st::settingsSectionPadding);
    
    autoStart->checkedChanges(
    ) | rpl::start_with_next([=](bool checked) {
        Core::App().settings().setBotAutoStart(checked);
        _showRestart.fire({});
    }, autoStart->lifetime());
    
    minimizeToTray->checkedChanges(
    ) | rpl::start_with_next([=](bool checked) {
        Core::App().settings().setBotMinimizeToTray(checked);
    }, minimizeToTray->lifetime());
    
    startMinimized->checkedChanges(
    ) | rpl::start_with_next([=](bool checked) {
        Core::App().settings().setBotStartMinimized(checked);
    }, startMinimized->lifetime());
    
    content->resizeToWidth(st::settingsWidth);
    
    Ui::AddSkip(content);
}

void BotClient::setupPrivacyOptions() {
    const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
    
    Ui::AddDividerText(content, tr::lng_settings_bot_privacy_about());
    
    const auto secureConnection = content->add(
        object_ptr<Ui::Checkbox>(
            content,
            tr::lng_settings_bot_secure_connection(),
            Core::App().settings().botSecureConnection(),
            st::settingsCheckbox),
        st::settingsSectionPadding);
    
    const auto localProxy = content->add(
        object_ptr<Ui::Checkbox>(
            content,
            tr::lng_settings_bot_local_proxy(),
            Core::App().settings().botLocalProxy(),
            st::settingsCheckbox),
        st::settingsSectionPadding);
    
    const auto encryptLogs = content->add(
        object_ptr<Ui::Checkbox>(
            content,
            tr::lng_settings_bot_encrypt_logs(),
            Core::App().settings().botEncryptLogs(),
            st::settingsCheckbox),
        st::settingsSectionPadding);
    
    secureConnection->checkedChanges(
    ) | rpl::start_with_next([=](bool checked) {
        Core::App().settings().setBotSecureConnection(checked);
        _showRestart.fire({});
    }, secureConnection->lifetime());
    
    localProxy->checkedChanges(
    ) | rpl::start_with_next([=](bool checked) {
        Core::App().settings().setBotLocalProxy(checked);
        _showRestart.fire({});
    }, localProxy->lifetime());
    
    encryptLogs->checkedChanges(
    ) | rpl::start_with_next([=](bool checked) {
        Core::App().settings().setBotEncryptLogs(checked);
        if (Core::App().settings().botLoggingEnabled()) {
            _showRestart.fire({});
        }
    }, encryptLogs->lifetime());
    
    content->resizeToWidth(st::settingsWidth);
    
    Ui::AddSkip(content);
}

void BotClient::setupUpdateOptions() {
    const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
    
    Ui::AddDividerText(content, tr::lng_settings_bot_updates_about());
    
    const auto autoUpdate = content->add(
        object_ptr<Ui::Checkbox>(
            content,
            tr::lng_settings_bot_auto_update(),
            Core::App().settings().botAutoUpdate(),
            st::settingsCheckbox),
        st::settingsSectionPadding);
    
    const auto updateChannelWrap = content->add(
        object_ptr<Ui::SlideWrap<Ui::VerticalLayout>>(
            content,
            object_ptr<Ui::VerticalLayout>(content)),
        st::settingsSectionPadding);
    
    const auto updateChannel = updateChannelWrap->entity();
    
    Ui::AddSkip(updateChannel);
    
    const auto updateChannelGroup = std::make_shared<Ui::RadioenumGroup<int>>(
        Core::App().settings().botUpdateChannel());
    
    const auto addChannelOption = [&](
            rpl::producer<QString> text,
            int value) {
        updateChannel->add(
            object_ptr<Ui::Radioenum<int>>(
                updateChannel,
                updateChannelGroup,
                value,
                std::move(text)),
            st::settingsSectionPadding);
    };
    
    addChannelOption(tr::lng_settings_bot_update_stable(), 0);
    addChannelOption(tr::lng_settings_bot_update_beta(), 1);
    addChannelOption(tr::lng_settings_bot_update_dev(), 2);
    
    updateChannelWrap->toggleOn(autoUpdate->checkedValue());
    
    const auto checkNow = content->add(
        object_ptr<Ui::RoundButton>(
            content,
            tr::lng_settings_bot_check_now(),
            st::settingsBotCheckButton),
        st::settingsSectionPadding);
    
    autoUpdate->checkedChanges(
    ) | rpl::start_with_next([=](bool checked) {
        Core::App().settings().setBotAutoUpdate(checked);
        updateChannelWrap->toggle(checked, anim::type::normal);
    }, autoUpdate->lifetime());
    
    updateChannelGroup->setChangedCallback([=](int channel) {
        Core::App().settings().setBotUpdateChannel(channel);
    });
    
    checkNow->setClickedCallback([=] {
        Ui::Toast::Show(Ui::Toast::Config{
            .text = { tr::lng_settings_bot_update_checking(tr::now) },
            .st = &st::defaultToast,
            .multiline = false,
        });
        
        // TODO: Add actual update check
        // For now, just show a message
        QTimer::singleShot(2000, [=] {
            Ui::Toast::Show(Ui::Toast::Config{
                .text = { tr::lng_settings_bot_update_latest(tr::now) },
                .st = &st::defaultToast,
                .multiline = false,
            });
        });
    });
    
    content->resizeToWidth(st::settingsWidth);
    
    Ui::AddSkip(content);
    
    // Connect the restart signal
    _showRestart.events(
    ) | rpl::start_with_next([=] {
        showRestartTooltip();
    }, lifetime());
}

void BotClient::showRestartTooltip() {
    Ui::Toast::Show(Ui::Toast::Config{
        .text = { tr::lng_settings_need_restart(tr::now) },
        .st = &st::defaultToast,
        .multiline = false,
    });
}

void BotClient::sectionSaveChanges(FnMut<void()> done) {
    // Ensure all settings are saved
    Core::App().saveSettings();
    
    // Flush log if logging is enabled
    if (Core::App().settings().botLoggingEnabled()) {
        BotLogger::flush();
    }
    
    done();
}

} // namespace Settings 
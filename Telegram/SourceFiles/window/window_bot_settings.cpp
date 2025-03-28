#include "window/window_bot_settings.h"
#include "api/api_bot_settings.h"
#include "ui/widgets/fields/input_field.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "ui/widgets/checkbox.h"
#include "ui/effects/animations.h"
#include "ui/text/text_utilities.h"
#include "ui/icons.h"
#include "ui/toast/toast.h"
#include "lang/lang_keys.h"
#include "main/main_session.h"
#include "data/data_bot_info.h"

namespace Window {

BotSettings::BotSettings(not_null<Main::Session*> session)
    : _session(session) {
    setupUI();
    _api = std::make_unique<Api::BotSettings>(session);

    connect(_api.get(), &Api::BotSettings::settingsUpdated, [=] {
        emit settingsChanged();
        hide();
    });

    connect(_api.get(), &Api::BotSettings::error, [=](const QString &error) {
        Ui::Toast::Show(error);
    });
}

BotSettings::~BotSettings() = default;

void BotSettings::setupUI() {
    _title.create(
        nullptr,
        tr::lng_bot_settings_title(tr::now),
        st::windowTitle);

    _closeButton.create(
        nullptr,
        st::windowCloseButton);

    _inlineModeCheck.create(
        nullptr,
        tr::lng_bot_settings_inline_mode(tr::now),
        false,
        st::defaultCheckbox);

    _groupModeCheck.create(
        nullptr,
        tr::lng_bot_settings_group_mode(tr::now),
        false,
        st::defaultCheckbox);

    _readMessagesCheck.create(
        nullptr,
        tr::lng_bot_settings_read_messages(tr::now),
        false,
        st::defaultCheckbox);

    _descriptionField.create(
        nullptr,
        st::defaultInputField,
        tr::lng_bot_settings_description_ph(tr::now));

    _saveButton.create(
        nullptr,
        tr::lng_bot_settings_save(tr::now),
        st::defaultButton);

    connect(_closeButton, &Ui::IconButton::clicked, [=] {
        hide();
    });

    connect(_saveButton, &Ui::RoundButton::clicked, [=] {
        saveSettings();
    });
}

void BotSettings::show() {
    if (_isVisible) return;
    _isVisible = true;
    _title->show();
    _closeButton->show();
    _inlineModeCheck->show();
    _groupModeCheck->show();
    _readMessagesCheck->show();
    _descriptionField->show();
    _saveButton->show();
    updateSettings();
    emit visibilityChanged(true);
}

void BotSettings::hide() {
    if (!_isVisible) return;
    _isVisible = false;
    _title->hide();
    _closeButton->hide();
    _inlineModeCheck->hide();
    _groupModeCheck->hide();
    _readMessagesCheck->hide();
    _descriptionField->hide();
    _saveButton->hide();
    emit visibilityChanged(false);
}

bool BotSettings::isVisible() const {
    return _isVisible;
}

void BotSettings::setBotInfo(const MTPBotInfo &info) {
    _currentInfo = info;
    updateSettings();
}

void BotSettings::updateSettings() {
    const auto &info = _currentInfo;
    if (!info.match([&](const MTPDbotInfo &data) {
        _inlineModeCheck->setChecked(data.vinline_placeholder().has_value());
        _groupModeCheck->setChecked(data.vcan_join_groups().has_value() && data.vcan_join_groups()->v);
        _readMessagesCheck->setChecked(data.vcan_read_all_group_messages().has_value() && data.vcan_read_all_group_messages()->v);
        
        if (data.vdescription().has_value()) {
            _descriptionField->setText(data.vdescription()->v);
        }
    })) {
        _inlineModeCheck->setChecked(false);
        _groupModeCheck->setChecked(false);
        _readMessagesCheck->setChecked(false);
        _descriptionField->setText(QString());
    }
}

void BotSettings::saveSettings() {
    _saveButton->setDisabled(true);
    _api->updateSettings(
        _descriptionField->getLastText().trimmed(),
        _inlineModeCheck->checked(),
        _groupModeCheck->checked(),
        _readMessagesCheck->checked()
    );
}

} // namespace Window 
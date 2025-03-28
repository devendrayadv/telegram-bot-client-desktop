#include "window/window_bot_mode.h"
#include "window/window_bot_settings.h"
#include "ui/widgets/fields/input_field.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "ui/effects/animations.h"
#include "ui/text/text_utilities.h"
#include "ui/icons.h"
#include "lang/lang_keys.h"
#include "main/main_session.h"
#include "data/data_bot_info.h"

namespace Window {

BotMode::BotMode(not_null<Main::Session*> session)
    : _session(session) {
    setupUI();
}

BotMode::~BotMode() = default;

void BotMode::setupUI() {
    _header.create(
        nullptr,
        tr::lng_bot_mode_header(tr::now),
        st::windowTitle);

    _toggleButton.create(
        nullptr,
        st::windowBotModeToggle);

    _restrictionsLabel.create(
        nullptr,
        QString(),
        st::windowDescription);

    _settingsButton.create(
        nullptr,
        tr::lng_bot_mode_settings(tr::now),
        st::windowBotModeSettings);

    _settings = std::make_unique<BotSettings>(_session);

    connect(_toggleButton, &Ui::IconButton::clicked, [=] {
        toggleVisibility();
    });

    connect(_settingsButton, &Ui::RoundButton::clicked, [=] {
        _settings->setBotInfo(_currentInfo);
        _settings->show();
    });

    connect(_settings.get(), &BotSettings::settingsChanged, [=] {
        // TODO: Update bot info from server
        updateRestrictions();
    });
}

void BotMode::show() {
    if (_isVisible) return;
    _isVisible = true;
    _header->show();
    _toggleButton->show();
    _restrictionsLabel->show();
    _settingsButton->show();
    emit visibilityChanged(true);
}

void BotMode::hide() {
    if (!_isVisible) return;
    _isVisible = false;
    _header->hide();
    _toggleButton->hide();
    _restrictionsLabel->hide();
    _settingsButton->hide();
    emit visibilityChanged(false);
}

bool BotMode::isVisible() const {
    return _isVisible;
}

void BotMode::setRestrictions(const MTPBotInfo &info) {
    _currentInfo = info;
    updateRestrictions();
}

void BotMode::updateRestrictions() {
    const auto &info = _currentInfo;
    if (!info.match([&](const MTPDbotInfo &data) {
        QStringList restrictionTexts;
        
        if (data.vcommands().has_value()) {
            restrictionTexts << tr::lng_bot_mode_commands(tr::now);
        }
        if (data.vdescription().has_value()) {
            restrictionTexts << tr::lng_bot_mode_description(tr::now);
        }
        if (data.vcan_join_groups().has_value() && data.vcan_join_groups()->v) {
            restrictionTexts << tr::lng_bot_mode_join_groups(tr::now);
        }
        if (data.vcan_read_all_group_messages().has_value() && data.vcan_read_all_group_messages()->v) {
            restrictionTexts << tr::lng_bot_mode_read_messages(tr::now);
        }
        if (data.vinline_placeholder().has_value()) {
            restrictionTexts << tr::lng_bot_mode_inline(tr::now);
        }

        const auto text = restrictionTexts.isEmpty()
            ? tr::lng_bot_mode_no_restrictions(tr::now)
            : restrictionTexts.join('\n');

        _restrictionsLabel->setText(text);
    })) {
        _restrictionsLabel->setText(tr::lng_bot_mode_no_info(tr::now));
    }

    emit restrictionsChanged();
}

void BotMode::toggleVisibility() {
    if (_isVisible) {
        hide();
    } else {
        show();
    }
}

} // namespace Window 
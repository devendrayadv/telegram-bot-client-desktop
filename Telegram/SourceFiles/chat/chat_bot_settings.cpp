#include "chat/chat_bot_settings.h"
#include "ui/widgets/fields/input_field.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "ui/widgets/checkbox.h"
#include "ui/effects/animations.h"
#include "ui/text/text_utilities.h"
#include "ui/icons.h"
#include "lang/lang_keys.h"
#include "main/main_session.h"
#include "data/data_bot_info.h"
#include "api/api_wrap.h"

namespace Chat {

BotSettings::BotSettings(not_null<Main::Session*> session)
    : _session(session) {
    setupUI();
}

BotSettings::~BotSettings() = default;

void BotSettings::setupUI() {
    _title.create(
        nullptr,
        tr::lng_bot_settings_title(tr::now),
        st::chatBotSettingsTitle);

    _closeButton.create(
        nullptr,
        st::chatBotSettingsClose);

    _description.create(
        nullptr,
        st::chatBotSettingsDescription,
        tr::lng_bot_settings_description_ph(tr::now));

    _inlineMode.create(
        nullptr,
        tr::lng_bot_settings_inline_mode(tr::now),
        st::chatBotSettingsCheckbox);

    _groupMode.create(
        nullptr,
        tr::lng_bot_settings_group_mode(tr::now),
        st::chatBotSettingsCheckbox);

    _readMessages.create(
        nullptr,
        tr::lng_bot_settings_read_messages(tr::now),
        st::chatBotSettingsCheckbox);

    _saveButton.create(
        nullptr,
        tr::lng_bot_settings_save(tr::now),
        st::chatBotSettingsSave);

    _cancelButton.create(
        nullptr,
        tr::lng_cancel(tr::now),
        st::chatBotSettingsCancel);

    connect(_closeButton, &Ui::IconButton::clicked, [=] {
        hide();
    });

    connect(_saveButton, &Ui::RoundButton::clicked, [=] {
        handleSave();
    });

    connect(_cancelButton, &Ui::RoundButton::clicked, [=] {
        handleCancel();
    });

    connect(_description, &Ui::InputField::changed, [=] {
        emit settingsChanged();
    });

    connect(_inlineMode, &Ui::Checkbox::changed, [=] {
        emit settingsChanged();
    });

    connect(_groupMode, &Ui::Checkbox::changed, [=] {
        emit settingsChanged();
    });

    connect(_readMessages, &Ui::Checkbox::changed, [=] {
        emit settingsChanged();
    });
}

void BotSettings::show() {
    if (_isVisible) return;
    _isVisible = true;
    _title->show();
    _closeButton->show();
    _description->show();
    _inlineMode->show();
    _groupMode->show();
    _readMessages->show();
    _saveButton->show();
    _cancelButton->show();
    updateLayout();
    emit visibilityChanged(true);
}

void BotSettings::hide() {
    if (!_isVisible) return;
    _isVisible = false;
    _title->hide();
    _closeButton->hide();
    _description->hide();
    _inlineMode->hide();
    _groupMode->hide();
    _readMessages->hide();
    _saveButton->hide();
    _cancelButton->hide();
    emit visibilityChanged(false);
}

bool BotSettings::isVisible() const {
    return _isVisible;
}

void BotSettings::setBotInfo(const MTPBotInfo &info) {
    _currentInfo = info;
    const auto &data = info.c_botInfo();
    
    _description->setText(qs(data.vdescription().value_or_empty()));
    _inlineMode->setChecked(data.is_inline_mode());
    _groupMode->setChecked(data.is_group_mode());
    _readMessages->setChecked(data.is_read_messages());
}

void BotSettings::updateLayout() {
    const auto top = st::chatBotSettingsTop;
    const auto left = st::chatBotSettingsLeft;
    const auto right = st::chatBotSettingsRight;

    _title->moveToLeft(left, top);
    _closeButton->moveToRight(right, top);

    const auto contentTop = top + st::chatBotSettingsTitleHeight + st::chatBotSettingsSpacing;
    _description->moveToLeft(left, contentTop);
    
    const auto checkboxTop = contentTop + st::chatBotSettingsDescriptionHeight + st::chatBotSettingsSpacing;
    _inlineMode->moveToLeft(left, checkboxTop);
    _groupMode->moveToLeft(left, checkboxTop + st::chatBotSettingsCheckboxHeight + st::chatBotSettingsSpacing);
    _readMessages->moveToLeft(left, checkboxTop + (st::chatBotSettingsCheckboxHeight + st::chatBotSettingsSpacing) * 2);

    const auto buttonTop = checkboxTop + (st::chatBotSettingsCheckboxHeight + st::chatBotSettingsSpacing) * 3 + st::chatBotSettingsSpacing;
    _saveButton->moveToLeft(left, buttonTop);
    _cancelButton->moveToRight(right, buttonTop);
}

void BotSettings::handleSave() {
    validateSettings();
    
    const auto description = _description->getLastText();
    const auto inlineMode = _inlineMode->checked();
    const auto groupMode = _groupMode->checked();
    const auto readMessages = _readMessages->checked();

    _session->api().updateBotSettings(
        description,
        inlineMode,
        groupMode,
        readMessages,
        [=](const MTPUpdates &result) {
            emit settingsSaved();
            hide();
        },
        [=](const MTP::Error &error) {
            emit settingsError(tr::lng_bot_settings_error_unknown(tr::now));
        });
}

void BotSettings::handleCancel() {
    hide();
}

void BotSettings::validateSettings() {
    const auto description = _description->getLastText();
    if (description.length() > 512) {
        emit settingsError(tr::lng_bot_settings_error_description_too_long(tr::now));
        return;
    }
}

} // namespace Chat 
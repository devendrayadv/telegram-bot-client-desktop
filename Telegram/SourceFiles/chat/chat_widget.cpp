#include "chat/chat_widget.h"
#include "ui/widgets/fields/input_field.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "ui/effects/animations.h"
#include "ui/text/text_utilities.h"
#include "ui/icons.h"
#include "lang/lang_keys.h"
#include "main/main_session.h"
#include "data/data_bot_info.h"

namespace Chat {

Widget::Widget(not_null<Main::Session*> session)
    : _session(session) {
    setupUI();
}

Widget::~Widget() = default;

void Widget::setupUI() {
    _title.create(
        nullptr,
        tr::lng_chat_title(tr::now),
        st::chatTitle);

    _menuButton.create(
        nullptr,
        st::chatMenuButton);

    _searchButton.create(
        nullptr,
        st::chatSearchButton);

    _settingsButton.create(
        nullptr,
        st::chatSettingsButton);

    _inputField.create(
        nullptr,
        st::chatInputField,
        tr::lng_chat_input_ph(tr::now));

    _sendButton.create(
        nullptr,
        tr::lng_chat_send(tr::now),
        st::chatSendButton);

    _restrictions = std::make_unique<BotRestrictions>(_session);
    _commands = std::make_unique<BotCommands>(_session);
    _commandHandler = std::make_unique<BotCommandHandler>(_session);
    _settings = std::make_unique<BotSettings>(_session);

    connect(_menuButton, &Ui::IconButton::clicked, [=] {
        if (_session->isBotMode()) {
            _commands->show();
        }
        // TODO: Show menu
    });

    connect(_searchButton, &Ui::IconButton::clicked, [=] {
        // TODO: Show search
    });

    connect(_settingsButton, &Ui::IconButton::clicked, [=] {
        if (_session->isBotMode()) {
            _settings->show();
        }
        // TODO: Show settings
    });

    connect(_inputField, &Ui::InputField::submitted, [=] {
        if (_restrictions && _restrictions->canSendMessage()) {
            const auto text = _inputField->getLastText();
            if (text.startsWith('/')) {
                _commandHandler->handleCommand(text.mid(1));
            } else {
                // TODO: Send message
            }
        }
    });

    connect(_sendButton, &Ui::RoundButton::clicked, [=] {
        if (_restrictions && _restrictions->canSendMessage()) {
            const auto text = _inputField->getLastText();
            if (text.startsWith('/')) {
                _commandHandler->handleCommand(text.mid(1));
            } else {
                // TODO: Send message
            }
        }
    });

    connect(_restrictions.get(), &BotRestrictions::restrictionsChanged, [=] {
        handleRestrictionsChanged();
    });

    connect(_restrictions.get(), &BotRestrictions::visibilityChanged, [=](bool visible) {
        handleVisibilityChanged(visible);
    });

    connect(_commands.get(), &BotCommands::visibilityChanged, [=](bool visible) {
        handleVisibilityChanged(visible);
    });

    connect(_commands.get(), &BotCommands::commandSelected, [=](const QString &command) {
        handleCommandSelected(command);
    });

    connect(_commandHandler.get(), &BotCommandHandler::commandProcessed, [=](const QString &command, const QString &response) {
        handleCommandProcessed(command, response);
    });

    connect(_commandHandler.get(), &BotCommandHandler::commandError, [=](const QString &command, const QString &error) {
        handleCommandError(command, error);
    });

    connect(_settings.get(), &BotSettings::visibilityChanged, [=](bool visible) {
        handleVisibilityChanged(visible);
    });

    connect(_settings.get(), &BotSettings::settingsChanged, [=] {
        handleSettingsChanged();
    });

    connect(_settings.get(), &BotSettings::settingsSaved, [=] {
        handleSettingsSaved();
    });

    connect(_settings.get(), &BotSettings::settingsError, [=](const QString &error) {
        handleSettingsError(error);
    });
}

void Widget::show() {
    if (_isVisible) return;
    _isVisible = true;
    _title->show();
    _menuButton->show();
    _searchButton->show();
    _settingsButton->show();
    _inputField->show();
    _sendButton->show();
    updateLayout();
    emit visibilityChanged(true);
}

void Widget::hide() {
    if (!_isVisible) return;
    _isVisible = false;
    _title->hide();
    _menuButton->hide();
    _searchButton->hide();
    _settingsButton->hide();
    _inputField->hide();
    _sendButton->hide();
    if (_restrictions) {
        _restrictions->hide();
    }
    if (_commands) {
        _commands->hide();
    }
    if (_settings) {
        _settings->hide();
    }
    emit visibilityChanged(false);
}

bool Widget::isVisible() const {
    return _isVisible;
}

void Widget::setBotInfo(const MTPBotInfo &info) {
    if (_restrictions) {
        _restrictions->setRestrictions(info);
    }
    if (_settings) {
        _settings->setBotInfo(info);
    }
}

void Widget::setBotCommands(const MTPVector<MTPBotCommand> &commands) {
    if (_commands) {
        _commands->setCommands(commands);
    }
}

void Widget::updateLayout() {
    const auto top = st::chatTitleHeight;
    const auto left = st::chatTitleLeft;
    const auto right = st::chatTitleRight;

    _title->moveToLeft(left, top);
    _menuButton->moveToLeft(left + _title->width() + st::chatTitleSpacing, top);
    _searchButton->moveToRight(right + st::chatTitleSpacing, top);
    _settingsButton->moveToRight(right, top);

    const auto inputTop = height() - st::chatInputHeight;
    _inputField->moveToLeft(left, inputTop);
    _sendButton->moveToRight(right, inputTop);

    if (_restrictions && _restrictions->isVisible()) {
        const auto restrictionsTop = top + st::chatTitleHeight + st::chatRestrictionsTop;
        _restrictions->moveToLeft(left, restrictionsTop);
    }

    if (_commands && _commands->isVisible()) {
        _commands->updateLayout();
    }

    if (_settings && _settings->isVisible()) {
        _settings->updateLayout();
    }
}

void Widget::handleRestrictionsChanged() {
    if (_restrictions) {
        _inputField->setEnabled(_restrictions->canSendMessage());
        _sendButton->setEnabled(_restrictions->canSendMessage());
    }
    emit restrictionsChanged();
}

void Widget::handleVisibilityChanged(bool visible) {
    updateLayout();
}

void Widget::handleCommandSelected(const QString &command) {
    _inputField->setText("/" + command + " ");
    _inputField->setFocus();
    emit commandSelected(command);
}

void Widget::handleCommandProcessed(const QString &command, const QString &response) {
    _inputField->clear();
    emit commandProcessed(command, response);
}

void Widget::handleCommandError(const QString &command, const QString &error) {
    emit commandError(command, error);
}

void Widget::handleSettingsChanged() {
    emit settingsChanged();
}

void Widget::handleSettingsSaved() {
    emit settingsSaved();
}

void Widget::handleSettingsError(const QString &error) {
    emit settingsError(error);
}

} // namespace Chat 
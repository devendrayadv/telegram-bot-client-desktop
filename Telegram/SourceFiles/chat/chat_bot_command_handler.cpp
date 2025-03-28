#include "chat/chat_bot_command_handler.h"
#include "ui/widgets/fields/input_field.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "ui/effects/animations.h"
#include "ui/text/text_utilities.h"
#include "ui/icons.h"
#include "lang/lang_keys.h"
#include "main/main_session.h"
#include "data/data_bot_info.h"
#include "api/api_wrap.h"

namespace Chat {

BotCommandHandler::BotCommandHandler(not_null<Main::Session*> session)
    : _session(session) {
}

BotCommandHandler::~BotCommandHandler() = default;

void BotCommandHandler::handleCommand(const QString &command) {
    if (_isProcessing) {
        emit commandError(command, tr::lng_bot_command_busy(tr::now));
        return;
    }

    _currentCommand = command;
    _isProcessing = true;
    emit commandStarted(command);

    processCommand(command);
}

void BotCommandHandler::handleCommandResponse(const QString &response) {
    if (!_isProcessing) return;

    _isProcessing = false;
    emit commandProcessed(_currentCommand, response);
    emit commandFinished(_currentCommand);
}

void BotCommandHandler::handleCommandError(const QString &error) {
    if (!_isProcessing) return;

    _isProcessing = false;
    emit commandError(_currentCommand, error);
    emit commandFinished(_currentCommand);
}

void BotCommandHandler::processCommand(const QString &command) {
    validateCommand(command);
    sendCommandToServer(command);
}

void BotCommandHandler::validateCommand(const QString &command) {
    if (command.isEmpty()) {
        handleCommandError(tr::lng_bot_command_empty(tr::now));
        return;
    }

    if (command.length() > 64) {
        handleCommandError(tr::lng_bot_command_too_long(tr::now));
        return;
    }

    if (!command.matches(QRegularExpression("^[a-zA-Z0-9_]+$"))) {
        handleCommandError(tr::lng_bot_command_invalid(tr::now));
        return;
    }
}

void BotCommandHandler::sendCommandToServer(const QString &command) {
    _session->api().sendBotCommand(command, [=](const MTPUpdates &result) {
        handleCommandResponse(tr::lng_bot_command_success(tr::now));
    }, [=](const MTP::Error &error) {
        handleCommandError(tr::lng_bot_command_error(tr::now));
    });
}

} // namespace Chat 
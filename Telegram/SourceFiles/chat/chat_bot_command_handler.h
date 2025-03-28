#pragma once

#include "base/object_ptr.h"
#include "ui/widgets/wrap.h"
#include "ui/effects/animations.h"
#include "main/main_session.h"
#include "data/data_bot_info.h"

namespace Ui {
class FlatLabel;
class IconButton;
class RoundButton;
class InputField;
} // namespace Ui

namespace Chat {

class BotCommandHandler : public QObject {
    Q_OBJECT

public:
    explicit BotCommandHandler(not_null<Main::Session*> session);
    ~BotCommandHandler();

    void handleCommand(const QString &command);
    void handleCommandResponse(const QString &response);
    void handleCommandError(const QString &error);

signals:
    void commandProcessed(const QString &command, const QString &response);
    void commandError(const QString &command, const QString &error);
    void commandStarted(const QString &command);
    void commandFinished(const QString &command);

private:
    void processCommand(const QString &command);
    void validateCommand(const QString &command);
    void sendCommandToServer(const QString &command);

    not_null<Main::Session*> _session;
    QString _currentCommand;
    bool _isProcessing = false;
};

} // namespace Chat 
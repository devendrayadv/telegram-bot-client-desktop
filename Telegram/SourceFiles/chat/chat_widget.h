#pragma once

#include "base/object_ptr.h"
#include "ui/widgets/wrap.h"
#include "ui/effects/animations.h"
#include "main/main_session.h"
#include "chat/chat_bot_restrictions.h"
#include "chat/chat_bot_commands.h"
#include "chat/chat_bot_command_handler.h"
#include "chat/chat_bot_settings.h"

namespace Ui {
class FlatLabel;
class IconButton;
class RoundButton;
class InputField;
} // namespace Ui

namespace Chat {

class Widget : public QObject {
    Q_OBJECT

public:
    explicit Widget(not_null<Main::Session*> session);
    ~Widget();

    void show();
    void hide();
    bool isVisible() const;

    void setBotInfo(const MTPBotInfo &info);
    void setBotCommands(const MTPVector<MTPBotCommand> &commands);

signals:
    void visibilityChanged(bool visible);
    void restrictionsChanged();
    void commandSelected(const QString &command);
    void commandProcessed(const QString &command, const QString &response);
    void commandError(const QString &command, const QString &error);
    void settingsChanged();
    void settingsSaved();
    void settingsError(const QString &error);

private:
    void setupUI();
    void updateLayout();
    void handleRestrictionsChanged();
    void handleVisibilityChanged(bool visible);
    void handleCommandSelected(const QString &command);
    void handleCommandProcessed(const QString &command, const QString &response);
    void handleCommandError(const QString &command, const QString &error);
    void handleSettingsChanged();
    void handleSettingsSaved();
    void handleSettingsError(const QString &error);

    not_null<Main::Session*> _session;
    std::unique_ptr<BotRestrictions> _restrictions;
    std::unique_ptr<BotCommands> _commands;
    std::unique_ptr<BotCommandHandler> _commandHandler;
    std::unique_ptr<BotSettings> _settings;
    object_ptr<Ui::FadeWrap<Ui::FlatLabel>> _title;
    object_ptr<Ui::FadeWrap<Ui::IconButton>> _menuButton;
    object_ptr<Ui::FadeWrap<Ui::IconButton>> _searchButton;
    object_ptr<Ui::FadeWrap<Ui::IconButton>> _settingsButton;
    object_ptr<Ui::FadeWrap<Ui::InputField>> _inputField;
    object_ptr<Ui::FadeWrap<Ui::RoundButton>> _sendButton;
    
    Ui::Animations::Simple _animations;
    bool _isVisible = false;
};

} // namespace Chat 
#pragma once

#include "base/object_ptr.h"
#include "ui/widgets/wrap.h"
#include "ui/effects/animations.h"
#include "main/main_session.h"
#include "window/window_bot_mode.h"

namespace Ui {
class FlatLabel;
class IconButton;
class RoundButton;
} // namespace Ui

namespace Window {

class Controller;

class Window : public QObject {
    Q_OBJECT

public:
    explicit Window(not_null<Controller*> controller);
    ~Window();

    void show();
    void hide();
    bool isVisible() const;

    void setBotInfo(const MTPBotInfo &info);

signals:
    void visibilityChanged(bool visible);
    void botModeChanged(bool enabled);

private:
    void setupUI();
    void updateLayout();

    not_null<Controller*> _controller;
    object_ptr<Ui::FadeWrap<Ui::FlatLabel>> _title;
    object_ptr<Ui::FadeWrap<Ui::IconButton>> _menuButton;
    object_ptr<Ui::FadeWrap<Ui::IconButton>> _searchButton;
    object_ptr<Ui::FadeWrap<Ui::IconButton>> _settingsButton;
    std::unique_ptr<BotMode> _botMode;
    
    Ui::Animations::Simple _animations;
    bool _isVisible = false;
};

} // namespace Window 
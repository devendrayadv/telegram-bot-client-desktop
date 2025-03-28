#pragma once

#include "base/object_ptr.h"
#include "ui/widgets/wrap.h"
#include "ui/effects/animations.h"
#include "main/main_session.h"
#include "window/window_bot_settings.h"

namespace Ui {
class FlatLabel;
class IconButton;
class RoundButton;
} // namespace Ui

namespace Window {

class BotMode : public QObject {
    Q_OBJECT

public:
    explicit BotMode(not_null<Main::Session*> session);
    ~BotMode();

    void show();
    void hide();
    bool isVisible() const;

    void setRestrictions(const MTPBotInfo &info);

signals:
    void visibilityChanged(bool visible);
    void restrictionsChanged();

private:
    void setupUI();
    void updateRestrictions();
    void toggleVisibility();

    not_null<Main::Session*> _session;
    object_ptr<Ui::FadeWrap<Ui::FlatLabel>> _header;
    object_ptr<Ui::FadeWrap<Ui::IconButton>> _toggleButton;
    object_ptr<Ui::FadeWrap<Ui::FlatLabel>> _restrictionsLabel;
    object_ptr<Ui::FadeWrap<Ui::RoundButton>> _settingsButton;
    std::unique_ptr<BotSettings> _settings;
    
    Ui::Animations::Simple _animations;
    bool _isVisible = false;
    MTPBotInfo _currentInfo;
};

} // namespace Window 
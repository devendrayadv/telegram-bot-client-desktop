#pragma once

#include "base/object_ptr.h"
#include "ui/widgets/wrap.h"
#include "ui/effects/animations.h"
#include "main/main_session.h"

namespace Ui {
class FlatLabel;
class IconButton;
class RoundButton;
} // namespace Ui

namespace Chat {

class BotRestrictions : public QObject {
    Q_OBJECT

public:
    explicit BotRestrictions(not_null<Main::Session*> session);
    ~BotRestrictions();

    void show();
    void hide();
    bool isVisible() const;

    void setRestrictions(const MTPBotInfo &info);
    bool canSendMessage() const;
    bool canSendMedia() const;
    bool canSendSticker() const;
    bool canSendGif() const;
    bool canSendGame() const;
    bool canUseInline() const;

signals:
    void restrictionsChanged();
    void visibilityChanged(bool visible);

private:
    void setupUI();
    void updateRestrictions();
    void checkRestrictions();

    not_null<Main::Session*> _session;
    object_ptr<Ui::FadeWrap<Ui::FlatLabel>> _restrictionsLabel;
    object_ptr<Ui::FadeWrap<Ui::IconButton>> _toggleButton;
    
    Ui::Animations::Simple _animations;
    bool _isVisible = false;
    MTPBotInfo _currentInfo;

    // Bot capabilities
    bool _canSendMessage = false;
    bool _canSendMedia = false;
    bool _canSendSticker = false;
    bool _canSendGif = false;
    bool _canSendGame = false;
    bool _canUseInline = false;
};

} // namespace Chat 
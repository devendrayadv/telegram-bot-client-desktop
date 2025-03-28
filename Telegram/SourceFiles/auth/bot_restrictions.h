#pragma once

#include "base/object_ptr.h"
#include "ui/widgets/wrap.h"
#include "ui/effects/animations.h"

namespace Ui {
class FlatLabel;
class RoundButton;
} // namespace Ui

namespace Auth {

class BotRestrictions : public QObject {
    Q_OBJECT

public:
    explicit BotRestrictions(not_null<Main::Session*> session);
    ~BotRestrictions();

    void showRestrictions();
    void hideRestrictions();

signals:
    void restrictionsChanged();

private:
    void setupUI();
    void updateRestrictions();

    not_null<Main::Session*> _session;
    object_ptr<Ui::FadeWrap<Ui::FlatLabel>> _restrictionsLabel;
    object_ptr<Ui::FadeWrap<Ui::RoundButton>> _continueButton;
    
    Ui::Animations::Simple _animations;
};

} // namespace Auth 
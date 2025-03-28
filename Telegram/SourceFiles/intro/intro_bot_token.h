#pragma once

#include "intro/intro_step.h"
#include "ui/widgets/fields.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "ui/wrap/fade_wrap.h"
#include "ui/effects/animations.h"
#include "auth/bot_restrictions.h"

namespace Intro {
namespace details {

class BotTokenStep : public Step {
public:
    BotTokenStep(
        QWidget *parent,
        not_null<Main::Account*> account,
        not_null<Data*> data);

    void finish(not_null<Main::Account*> account) override;
    void setInnerFocus() override;
    void activate() override;
    void cancelled() override;
    void submit() override;

    [[nodiscard]] bool hasBack() const override;
    [[nodiscard]] rpl::producer<QString> nextButtonText() const override;

protected:
    void resizeEvent(QResizeEvent *e) override;
    void paintEvent(QPaintEvent *e) override;

private:
    void setupControls();
    void showError(const QString &error);
    void hideError();
    void validateToken(const QString &token);

    object_ptr<Ui::FadeWrap<Ui::FlatLabel>> _description;
    object_ptr<Ui::FadeWrap<Ui::InputField>> _token;
    object_ptr<Ui::FadeWrap<Ui::FlatLabel>> _error;
    object_ptr<Ui::FadeWrap<Ui::RoundButton>> _submit;
    std::unique_ptr<Auth::BotRestrictions> _restrictions;

    Ui::Animations::Simple _errorAnimation;
    bool _errorShown = false;
};

} // namespace details
} // namespace Intro 
#include "intro/intro_bot_token.h"
#include "intro/intro_widget.h"
#include "ui/widgets/fields/input_field.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "ui/effects/animations.h"
#include "ui/text/text_utilities.h"
#include "ui/toast/toast.h"
#include "lang/lang_keys.h"
#include "auth/bot_auth.h"
#include "auth/bot_restrictions.h"
#include "main/main_session.h"

namespace Intro {
namespace details {

BotTokenStep::BotTokenStep(
    QWidget *parent,
    not_null<Main::Account*> account,
    not_null<Data*> data)
    : Step(parent, account, data) {
    _description.create(
        this,
        tr::lng_bot_token_description(tr::now),
        st::introDescription);

    _token.create(
        this,
        st::introInput,
        tr::lng_bot_token_ph(tr::now));

    _submit.create(
        this,
        tr::lng_bot_token_submit(tr::now),
        st::introNext);

    _error.create(
        this,
        QString(),
        st::introError);

    _restrictions = std::make_unique<Auth::BotRestrictions>(_account->session());

    connect(_token, &Ui::InputField::submitted, [=] { submitToken(); });
    connect(_submit, &Ui::RoundButton::clicked, [=] { submitToken(); });
    connect(_restrictions.get(), &Auth::BotRestrictions::restrictionsChanged, [=] {
        finish(anim::type::normal);
    });
}

void BotTokenStep::submitToken() {
    const auto token = _token->getLastText().trimmed();
    if (token.isEmpty()) {
        showError(tr::lng_bot_token_empty(tr::now));
        return;
    }

    // Basic token format validation
    if (!token.contains(':')) {
        showError(tr::lng_bot_token_invalid_format(tr::now));
        return;
    }

    hideError();
    _submit->setDisabled(true);

    // Create bot auth instance and start authentication
    auto botAuth = std::make_unique<Auth::BotAuth>(_account->session());
    
    // Connect all signals
    connect(botAuth.get(), &Auth::BotAuth::authorized, [=] {
        _submit->setEnabled(true);
        _restrictions->showRestrictions();
    });

    connect(botAuth.get(), &Auth::BotAuth::tokenInvalid, [=] {
        _submit->setEnabled(true);
        showError(tr::lng_bot_token_invalid(tr::now));
    });

    connect(botAuth.get(), &Auth::BotAuth::tokenExpired, [=] {
        _submit->setEnabled(true);
        showError(tr::lng_bot_token_expired(tr::now));
    });

    connect(botAuth.get(), &Auth::BotAuth::networkError, [=] {
        _submit->setEnabled(true);
        showError(tr::lng_bot_token_network_error(tr::now));
    });

    connect(botAuth.get(), &Auth::BotAuth::error, [=](const QString &error) {
        _submit->setEnabled(true);
        showError(error);
    });

    botAuth->start(token);
}

void BotTokenStep::showError(const QString &error) {
    _error->setText(error);
    _error->show();
    _token->showError();
}

void BotTokenStep::hideError() {
    _error->hide();
    _token->hideError();
}

void BotTokenStep::setInnerFocus() {
    _token->setFocus();
}

void BotTokenStep::activate() {
    _token->setFocus();
}

void BotTokenStep::cancelled() {
    _token->clear();
    hideError();
}

void BotTokenStep::submit() {
    submitToken();
}

void BotTokenStep::resizeEvent(QResizeEvent *e) {
    Step::resizeEvent(e);

    _description->moveToLeft(
        st::introStepIconLeft,
        st::introStepIconTop + st::introStepIconHeight + st::introStepTextTop);

    _token->moveToLeft(
        st::introStepIconLeft,
        _description->y() + _description->height() + st::introStepTextTop);

    _error->moveToLeft(
        st::introStepIconLeft,
        _token->y() + _token->height() + st::introStepTextTop);

    _submit->moveToLeft(
        st::introStepIconLeft,
        _error->y() + _error->height() + st::introStepTextTop);

    if (_restrictions) {
        _restrictions->moveToLeft(
            st::introStepIconLeft,
            _submit->y() + _submit->height() + st::introStepTextTop);
    }
}

void BotTokenStep::paintContent(QPainter &p) {
    // Draw any additional content if needed
}

void BotTokenStep::finish(anim::type animated) {
    _token->clear();
    hideError();
    if (_restrictions) {
        _restrictions->hideRestrictions();
    }
    Step::finish(animated);
}

} // namespace details
} // namespace Intro 
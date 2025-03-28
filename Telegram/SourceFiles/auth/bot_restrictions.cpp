#include "auth/bot_restrictions.h"
#include "ui/widgets/fields/input_field.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "ui/effects/animations.h"
#include "ui/text/text_utilities.h"
#include "lang/lang_keys.h"
#include "main/main_session.h"

namespace Auth {

BotRestrictions::BotRestrictions(not_null<Main::Session*> session)
    : _session(session) {
    setupUI();
}

BotRestrictions::~BotRestrictions() = default;

void BotRestrictions::setupUI() {
    _restrictionsLabel.create(
        nullptr,
        tr::lng_bot_restrictions_text(tr::now),
        st::introDescription);

    _continueButton.create(
        nullptr,
        tr::lng_bot_continue(tr::now),
        st::introNext);

    connect(_continueButton, &Ui::RoundButton::clicked, [=] {
        hideRestrictions();
        emit restrictionsChanged();
    });
}

void BotRestrictions::showRestrictions() {
    _restrictionsLabel->show();
    _continueButton->show();
    updateRestrictions();
}

void BotRestrictions::hideRestrictions() {
    _restrictionsLabel->hide();
    _continueButton->hide();
}

void BotRestrictions::updateRestrictions() {
    // Update UI based on current bot restrictions
    const auto &restrictions = _session->data().botRestrictions();
    
    QStringList restrictionTexts;
    if (restrictions.canSendMessages) {
        restrictionTexts << tr::lng_bot_restriction_messages(tr::now);
    }
    if (restrictions.canSendMedia) {
        restrictionTexts << tr::lng_bot_restriction_media(tr::now);
    }
    if (restrictions.canSendStickers) {
        restrictionTexts << tr::lng_bot_restriction_stickers(tr::now);
    }
    if (restrictions.canSendGifs) {
        restrictionTexts << tr::lng_bot_restriction_gifs(tr::now);
    }
    if (restrictions.canSendGames) {
        restrictionTexts << tr::lng_bot_restriction_games(tr::now);
    }
    if (restrictions.canUseInline) {
        restrictionTexts << tr::lng_bot_restriction_inline(tr::now);
    }

    const auto text = restrictionTexts.isEmpty()
        ? tr::lng_bot_restrictions_none(tr::now)
        : restrictionTexts.join('\n');

    _restrictionsLabel->setText(text);
}

} // namespace Auth 
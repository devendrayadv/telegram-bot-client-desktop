#include "chat/chat_bot_restrictions.h"
#include "ui/widgets/fields/input_field.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "ui/effects/animations.h"
#include "ui/text/text_utilities.h"
#include "ui/icons.h"
#include "lang/lang_keys.h"
#include "main/main_session.h"
#include "data/data_bot_info.h"

namespace Chat {

BotRestrictions::BotRestrictions(not_null<Main::Session*> session)
    : _session(session) {
    setupUI();
}

BotRestrictions::~BotRestrictions() = default;

void BotRestrictions::setupUI() {
    _restrictionsLabel.create(
        nullptr,
        QString(),
        st::chatRestrictionsLabel);

    _toggleButton.create(
        nullptr,
        st::chatRestrictionsToggle);

    connect(_toggleButton, &Ui::IconButton::clicked, [=] {
        if (_isVisible) {
            hide();
        } else {
            show();
        }
    });
}

void BotRestrictions::show() {
    if (_isVisible) return;
    _isVisible = true;
    _restrictionsLabel->show();
    _toggleButton->show();
    emit visibilityChanged(true);
}

void BotRestrictions::hide() {
    if (!_isVisible) return;
    _isVisible = false;
    _restrictionsLabel->hide();
    _toggleButton->hide();
    emit visibilityChanged(false);
}

bool BotRestrictions::isVisible() const {
    return _isVisible;
}

void BotRestrictions::setRestrictions(const MTPBotInfo &info) {
    _currentInfo = info;
    updateRestrictions();
}

bool BotRestrictions::canSendMessage() const {
    return _canSendMessage;
}

bool BotRestrictions::canSendMedia() const {
    return _canSendMedia;
}

bool BotRestrictions::canSendSticker() const {
    return _canSendSticker;
}

bool BotRestrictions::canSendGif() const {
    return _canSendGif;
}

bool BotRestrictions::canSendGame() const {
    return _canSendGame;
}

bool BotRestrictions::canUseInline() const {
    return _canUseInline;
}

void BotRestrictions::updateRestrictions() {
    const auto &info = _currentInfo;
    if (!info.match([&](const MTPDbotInfo &data) {
        QStringList restrictionTexts;
        
        _canSendMessage = data.vcan_send_messages().has_value() && data.vcan_send_messages()->v;
        _canSendMedia = data.vcan_send_media().has_value() && data.vcan_send_media()->v;
        _canSendSticker = data.vcan_send_stickers().has_value() && data.vcan_send_stickers()->v;
        _canSendGif = data.vcan_send_gifs().has_value() && data.vcan_send_gifs()->v;
        _canSendGame = data.vcan_send_games().has_value() && data.vcan_send_games()->v;
        _canUseInline = data.vinline_placeholder().has_value();

        if (_canSendMessage) {
            restrictionTexts << tr::lng_bot_restriction_messages(tr::now);
        }
        if (_canSendMedia) {
            restrictionTexts << tr::lng_bot_restriction_media(tr::now);
        }
        if (_canSendSticker) {
            restrictionTexts << tr::lng_bot_restriction_stickers(tr::now);
        }
        if (_canSendGif) {
            restrictionTexts << tr::lng_bot_restriction_gifs(tr::now);
        }
        if (_canSendGame) {
            restrictionTexts << tr::lng_bot_restriction_games(tr::now);
        }
        if (_canUseInline) {
            restrictionTexts << tr::lng_bot_restriction_inline(tr::now);
        }

        const auto text = restrictionTexts.isEmpty()
            ? tr::lng_bot_restrictions_none(tr::now)
            : restrictionTexts.join('\n');

        _restrictionsLabel->setText(text);
    })) {
        _canSendMessage = false;
        _canSendMedia = false;
        _canSendSticker = false;
        _canSendGif = false;
        _canSendGame = false;
        _canUseInline = false;
        _restrictionsLabel->setText(tr::lng_bot_restrictions_none(tr::now));
    }

    emit restrictionsChanged();
}

void BotRestrictions::checkRestrictions() {
    updateRestrictions();
}

} // namespace Chat 
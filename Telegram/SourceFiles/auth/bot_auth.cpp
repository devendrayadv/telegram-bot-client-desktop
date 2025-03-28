#include "auth/bot_auth.h"
#include "main/main_session.h"
#include "mtproto/mtproto_proxy.h"
#include "mtproto/sender.h"
#include "ui/boxes/confirm_box.h"
#include "ui/toast/toast.h"
#include "lang/lang_keys.h"

namespace Auth {

BotAuth::BotAuth(not_null<Main::Session*> session)
    : _session(session) {
}

BotAuth::~BotAuth() {
    if (_requestId) {
        _session->sender().request(_requestId).cancel();
    }
}

void BotAuth::start(const QString &botToken) {
    _botToken = botToken;
    importBotAuthorization(botToken);
}

bool BotAuth::isBotSession() const {
    return _isBotSession;
}

QString BotAuth::getBotToken() const {
    return _botToken;
}

void BotAuth::importBotAuthorization(const QString &botToken) {
    _requestId = _session->sender().request(
        MTPauth_ImportBotAuthorization(
            MTP_int(0), // flags
            MTP_int(_session->apiId()),
            MTP_string(_session->apiHash()),
            MTP_string(botToken)
        )
    ).done([=](const MTPauth_Authorization &result) {
        handleBotAuthorization(result);
    }).fail([=](const MTP::Error &error) {
        handleError(error);
    }).send();
}

void BotAuth::handleBotAuthorization(const MTPauth_Authorization &result) {
    _isBotSession = true;
    emit authorized();
}

void BotAuth::handleError(const MTP::Error &error) {
    const auto &type = error.type();
    
    if (type == u"BOT_TOKEN_INVALID"_q) {
        emit tokenInvalid();
    } else if (type == u"BOT_TOKEN_EXPIRED"_q) {
        emit tokenExpired();
    } else if (type == u"NETWORK_MIGRATE"_q || type == u"PHONE_MIGRATE"_q) {
        emit networkError();
    } else {
        emit this->error(error.description());
    }
}

} // namespace Auth 
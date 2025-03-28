#include "api/api_bot_settings.h"
#include "api/api_common.h"
#include "api/api_requests.h"
#include "api/api_authorization.h"
#include "main/main_session.h"
#include "lang/lang_keys.h"
#include "ui/toast/toast.h"

namespace Api {

BotSettings::BotSettings(not_null<Main::Session*> session)
    : _session(session) {
}

BotSettings::~BotSettings() = default;

void BotSettings::updateSettings(
        const QString &description,
        bool inlineMode,
        bool groupMode,
        bool readMessages) {
    const auto botId = _session->data().botId();
    if (!botId) {
        emit error(tr::lng_bot_settings_error_not_bot(tr::now));
        return;
    }

    const auto request = MTPbots_SetBotInfo(
        MTP_inputBotID(botId),
        MTP_string(description),
        MTP_bool(inlineMode),
        MTP_bool(groupMode),
        MTP_bool(readMessages)
    );

    _session->api().request(request).done([=](const MTPBool &result) {
        emit settingsUpdated();
    }).fail([=](const MTP::Error &error) {
        handleError(error);
    }).send();
}

void BotSettings::handleError(const MTP::Error &error) {
    QString errorText;
    switch (error.type()) {
        case qstr("BOT_INVALID"):
            errorText = tr::lng_bot_settings_error_invalid(tr::now);
            break;
        case qstr("BOT_DESCRIPTION_TOO_LONG"):
            errorText = tr::lng_bot_settings_error_description_too_long(tr::now);
            break;
        case qstr("BOT_SETTINGS_INVALID"):
            errorText = tr::lng_bot_settings_error_invalid_settings(tr::now);
            break;
        default:
            errorText = tr::lng_bot_settings_error_unknown(tr::now);
            break;
    }
    emit this->error(errorText);
}

} // namespace Api 
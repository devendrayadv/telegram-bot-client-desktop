#pragma once

#include "base/basic_types.h"
#include "mtproto/sender.h"
#include "base/object_ptr.h"

namespace Main {
class Session;
} // namespace Main

namespace Auth {

class BotAuth final : public QObject {
    Q_OBJECT

public:
    explicit BotAuth(not_null<Main::Session*> session);
    ~BotAuth();

    // Start bot authentication process
    void start(const QString &botToken);
    
    // Check if current session is a bot session
    [[nodiscard]] bool isBotSession() const;
    
    // Get current bot token if authenticated as bot
    [[nodiscard]] QString getBotToken() const;

signals:
    void authorized();
    void error(const QString &error);
    void tokenInvalid();
    void tokenExpired();
    void networkError();

private:
    void importBotAuthorization(const QString &botToken);
    void handleBotAuthorization(const MTPauth_Authorization &result);
    void handleError(const MTP::Error &error);

    not_null<Main::Session*> _session;
    QString _botToken;
    bool _isBotSession = false;
    mtpRequestId _requestId = 0;
};

} // namespace Auth 
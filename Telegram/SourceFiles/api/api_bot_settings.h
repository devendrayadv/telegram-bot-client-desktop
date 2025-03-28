#pragma once

#include "base/object_ptr.h"
#include "api/api_common.h"
#include "main/main_session.h"

namespace Api {

class BotSettings : public QObject {
    Q_OBJECT

public:
    explicit BotSettings(not_null<Main::Session*> session);
    ~BotSettings();

    void updateSettings(
        const QString &description,
        bool inlineMode,
        bool groupMode,
        bool readMessages);

signals:
    void settingsUpdated();
    void error(const QString &error);

private:
    void handleError(const MTP::Error &error);

    not_null<Main::Session*> _session;
};

} // namespace Api 
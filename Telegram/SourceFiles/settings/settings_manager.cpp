#include "settings/settings_bot_client.h"

// In the createSection function, add:
else if (type == Section::Type::BotClient) {
    result = Box::make<BotClient>(parent, controller);
} 
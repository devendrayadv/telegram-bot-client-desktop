#pragma once

#include "settings/settings_common.h"

namespace Main {
class Session;
} // namespace Main

namespace Settings {

class BotClient : public Section {
public:
    BotClient(
        QWidget *parent,
        not_null<Window::SessionController*> controller);

    void sectionSaveChanges(FnMut<void()> done) override;

private:
    void setupContent();
    void showRestartTooltip();

    // Bot client mode settings
    void setupLoggingOptions();
    void setupAutoStartOptions();
    void setupPrivacyOptions();
    void setupUpdateOptions();
    
    const not_null<Window::SessionController*> _controller;
    const not_null<Main::Session*> _session;
    rpl::event_stream<> _showRestart;
};
} // namespace Settings 
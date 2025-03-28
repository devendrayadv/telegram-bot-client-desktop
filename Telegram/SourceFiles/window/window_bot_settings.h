#pragma once

#include "base/object_ptr.h"
#include "ui/widgets/wrap.h"
#include "ui/effects/animations.h"
#include "main/main_session.h"
#include "api/api_bot_settings.h"

namespace Ui {
class FlatLabel;
class IconButton;
class RoundButton;
class Checkbox;
class InputField;
} // namespace Ui

namespace Window {

class BotSettings : public QObject {
    Q_OBJECT

public:
    explicit BotSettings(not_null<Main::Session*> session);
    ~BotSettings();

    void show();
    void hide();
    bool isVisible() const;

    void setBotInfo(const MTPBotInfo &info);

signals:
    void settingsChanged();
    void visibilityChanged(bool visible);

private:
    void setupUI();
    void updateSettings();
    void saveSettings();

    not_null<Main::Session*> _session;
    std::unique_ptr<Api::BotSettings> _api;
    object_ptr<Ui::FadeWrap<Ui::FlatLabel>> _title;
    object_ptr<Ui::FadeWrap<Ui::IconButton>> _closeButton;
    object_ptr<Ui::FadeWrap<Ui::Checkbox>> _inlineModeCheck;
    object_ptr<Ui::FadeWrap<Ui::Checkbox>> _groupModeCheck;
    object_ptr<Ui::FadeWrap<Ui::Checkbox>> _readMessagesCheck;
    object_ptr<Ui::FadeWrap<Ui::InputField>> _descriptionField;
    object_ptr<Ui::FadeWrap<Ui::RoundButton>> _saveButton;
    
    Ui::Animations::Simple _animations;
    bool _isVisible = false;
    MTPBotInfo _currentInfo;
};

} // namespace Window 
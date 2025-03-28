#pragma once

#include "base/object_ptr.h"
#include "ui/widgets/wrap.h"
#include "ui/effects/animations.h"
#include "main/main_session.h"
#include "data/data_bot_info.h"

namespace Ui {
class FlatLabel;
class IconButton;
class RoundButton;
class InputField;
class Checkbox;
} // namespace Ui

namespace Chat {

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
    void visibilityChanged(bool visible);
    void settingsChanged();
    void settingsSaved();
    void settingsError(const QString &error);

private:
    void setupUI();
    void updateLayout();
    void handleSave();
    void handleCancel();
    void validateSettings();

    not_null<Main::Session*> _session;
    object_ptr<Ui::FadeWrap<Ui::FlatLabel>> _title;
    object_ptr<Ui::FadeWrap<Ui::IconButton>> _closeButton;
    object_ptr<Ui::FadeWrap<Ui::InputField>> _description;
    object_ptr<Ui::FadeWrap<Ui::Checkbox>> _inlineMode;
    object_ptr<Ui::FadeWrap<Ui::Checkbox>> _groupMode;
    object_ptr<Ui::FadeWrap<Ui::Checkbox>> _readMessages;
    object_ptr<Ui::FadeWrap<Ui::RoundButton>> _saveButton;
    object_ptr<Ui::FadeWrap<Ui::RoundButton>> _cancelButton;
    
    Ui::Animations::Simple _animations;
    bool _isVisible = false;
    MTPBotInfo _currentInfo;
};

} // namespace Chat 
#pragma once

#include "base/unique_qptr.h"
#include "base/object_ptr.h"
#include "mtproto/sender.h"

class QListView;

namespace Main {
class Session;
} // namespace Main

namespace Ui {
class IconButton;
class RoundButton;
class ScrollArea;
} // namespace Ui

namespace Data {
struct BotInfo;
} // namespace Data

namespace Chat {

struct BotCommand {
    QString command;
    QString description;
};

class BotCommands : public QObject {
    Q_OBJECT

public:
    BotCommands(not_null<Main::Session*> session);
    ~BotCommands();

    void show();
    void hide();
    bool isVisible() const;

    void setCommands(const MTPVector<MTPBotCommand> &commands);
    void updateLayout();

signals:
    void visibilityChanged(bool visible);
    void commandSelected(const QString &command);
    void refreshRequested();

private:
    void setupUI();
    void updateOpacity();

    const not_null<Main::Session*> _session;
    
    struct Private;
    std::unique_ptr<Private> _private;
};

} // namespace Chat

// Register BotCommand as a metatype for use with QVariant
Q_DECLARE_METATYPE(Chat::BotCommand) 
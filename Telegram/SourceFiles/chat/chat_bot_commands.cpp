#include "chat/chat_bot_commands.h"
#include "ui/widgets/fields/input_field.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "ui/widgets/scroll_area.h"
#include "ui/widgets/shadow.h"
#include "ui/effects/animations.h"
#include "ui/painter.h"
#include "ui/text/text_utilities.h"
#include "ui/icons.h"
#include "lang/lang_keys.h"
#include "main/main_session.h"
#include "data/data_bot_info.h"
#include "styles/style_chat.h"
#include "chat/chat_bot_logger.h"

#include <QScrollBar>
#include <QVBoxLayout>
#include <QTimer>
#include <QCache>

namespace Chat {

namespace {
// Cache size for command items
constexpr int kCommandsCacheSize = 100;

// Batch size for lazy loading commands
constexpr int kCommandsBatchSize = 20;

// Command item heights cache
QCache<QString, int> CommandHeightCache(kCommandsCacheSize);

// Virtual list model for commands
class CommandsListModel : public QAbstractListModel {
public:
    CommandsListModel(QObject *parent = nullptr) 
        : QAbstractListModel(parent) {}
    
    void setCommands(const QVector<BotCommand> &commands) {
        beginResetModel();
        _commands = commands;
        endResetModel();
        
        // Pre-cache the first batch of commands
        QTimer::singleShot(0, [this] {
            preCacheItems(0, qMin(kCommandsBatchSize, _commands.size()));
        });
    }
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        return _commands.size();
    }
    
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() >= _commands.size())
            return QVariant();
            
        if (role == Qt::DisplayRole) {
            return QVariant::fromValue(_commands[index.row()]);
        }
        
        return QVariant();
    }
    
    // Pre-cache item heights for visible range
    void preCacheItems(int startIndex, int endIndex) {
        if (startIndex < 0 || endIndex > _commands.size())
            return;
            
        for (int i = startIndex; i < endIndex; ++i) {
            const auto &cmd = _commands[i];
            if (!CommandHeightCache.contains(cmd.command)) {
                // Calculate and cache the height
                const auto height = calculateCommandHeight(cmd);
                CommandHeightCache.insert(cmd.command, new int(height));
            }
        }
    }
    
private:
    int calculateCommandHeight(const BotCommand &command) const {
        // Calculate the height based on text length and wrapping
        const auto fontMetrics = QFontMetrics(st::botCommandsItem.font);
        const auto descHeight = fontMetrics.height() * 
            (1 + (command.description.length() / 40)); // Approximate line count
        return st::botCommandsItemHeight + descHeight;
    }
    
    QVector<BotCommand> _commands;
};

// Delegate for rendering command items
class CommandItemDelegate : public QAbstractItemDelegate {
public:
    CommandItemDelegate(QObject *parent = nullptr)
        : QAbstractItemDelegate(parent) {}
        
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override {
        painter->save();
        
        const auto command = index.data().value<BotCommand>();
        const auto isHovered = option.state & QStyle::State_MouseOver;
        const auto isSelected = option.state & QStyle::State_Selected;
        
        // Background
        if (isSelected) {
            painter->fillRect(option.rect, st::botCommandsItemSelectedBg);
        } else if (isHovered) {
            painter->fillRect(option.rect, st::botCommandsItemHoverBg);
        }
        
        // Command text
        const auto textLeft = option.rect.left() + st::botCommandsItemPadding.left();
        const auto textTop = option.rect.top() + st::botCommandsItemPadding.top();
        
        painter->setFont(st::botCommandsItemCommandFont);
        painter->setPen(isSelected ? st::botCommandsItemSelectedFg : st::botCommandsItemFg);
        painter->drawText(
            textLeft,
            textTop + st::botCommandsItemCommandFont.ascent,
            "/" + command.command);
            
        // Description text
        painter->setFont(st::botCommandsItemDescriptionFont);
        painter->setPen(isSelected ? st::botCommandsItemSelectedDescriptionFg : st::botCommandsItemDescriptionFg);
        
        const auto descRect = QRect(
            textLeft,
            textTop + st::botCommandsItemCommandFont.height + st::botCommandsItemDescriptionSkip,
            option.rect.width() - st::botCommandsItemPadding.left() - st::botCommandsItemPadding.right(),
            option.rect.height() - st::botCommandsItemPadding.top() - st::botCommandsItemPadding.bottom() - 
                st::botCommandsItemCommandFont.height - st::botCommandsItemDescriptionSkip);
                
        painter->drawText(descRect, Qt::TextWordWrap, command.description);
        
        painter->restore();
    }
    
    QSize sizeHint(const QStyleOptionViewItem &option,
                  const QModelIndex &index) const override {
        const auto command = index.data().value<BotCommand>();
        
        // Use cached height if available
        if (CommandHeightCache.contains(command.command)) {
            return QSize(option.rect.width(), *CommandHeightCache[command.command]);
        }
        
        // Calculate height based on text wrapping
        const auto fontMetrics = QFontMetrics(st::botCommandsItemDescriptionFont);
        const auto descWidth = option.rect.width() - 
            st::botCommandsItemPadding.left() - 
            st::botCommandsItemPadding.right();
        
        const auto descHeight = fontMetrics.boundingRect(
            0, 0, descWidth, 1000,
            Qt::TextWordWrap, command.description).height();
            
        const auto height = st::botCommandsItemPadding.top() + 
            st::botCommandsItemCommandFont.height + 
            st::botCommandsItemDescriptionSkip + 
            descHeight + 
            st::botCommandsItemPadding.bottom();
            
        // Cache the calculated height
        CommandHeightCache.insert(command.command, new int(height));
        
        return QSize(option.rect.width(), height);
    }
};

} // namespace

struct BotCommands::Private {
    CommandsListModel *model = nullptr;
    Ui::ScrollArea *scrollArea = nullptr;
    QListView *listView = nullptr;
    Ui::IconButton *closeButton = nullptr;
    Ui::RoundButton *refreshButton = nullptr;
    bool visible = false;
    Ui::Animations::Simple visibilityAnimation;
    QVector<BotCommand> commands;
};

BotCommands::BotCommands(not_null<Main::Session*> session)
    : _session(session)
    , _private(std::make_unique<Private>()) {
    setupUI();
    BotLogger::info("BotCommands", "Initialized bot commands component");
}

BotCommands::~BotCommands() = default;

void BotCommands::setupUI() {
    _private->model = new CommandsListModel(this);
    
    _private->scrollArea = new Ui::ScrollArea(nullptr, st::botCommandsScroll);
    _private->scrollArea->setWidgetResizable(true);
    
    _private->listView = new QListView();
    _private->listView->setModel(_private->model);
    _private->listView->setItemDelegate(new CommandItemDelegate(this));
    _private->listView->setSelectionMode(QAbstractItemView::SingleSelection);
    _private->listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    _private->listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _private->listView->setStyleSheet("QListView { background: transparent; border: none; }");
    
    // Implement lazy loading when scrolling
    connect(_private->listView->verticalScrollBar(), &QScrollBar::valueChanged, [this](int value) {
        const auto visibleItems = _private->listView->height() / st::botCommandsItemHeight;
        const auto startIndex = value / st::botCommandsItemHeight;
        const auto endIndex = qMin(startIndex + visibleItems * 2, _private->commands.size());
        _private->model->preCacheItems(startIndex, endIndex);
    });
    
    connect(_private->listView, &QListView::clicked, [this](const QModelIndex &index) {
        if (index.isValid()) {
            const auto command = index.data().value<BotCommand>();
            emit commandSelected(command.command);
            hide();
        }
    });
    
    _private->scrollArea->setWidget(_private->listView);
    
    _private->closeButton = new Ui::IconButton(nullptr, st::botCommandsClose);
    connect(_private->closeButton, &Ui::IconButton::clicked, [this] {
        hide();
    });
    
    _private->refreshButton = new Ui::RoundButton(
        nullptr,
        tr::lng_bot_commands_refresh(tr::now),
        st::botCommandsRefresh);
    connect(_private->refreshButton, &Ui::RoundButton::clicked, [this] {
        BotLogger::info("BotCommands", "Refreshing commands");
        // Refresh commands from server
        emit refreshRequested();
    });
    
    hide();
}

void BotCommands::show() {
    if (_private->visible)
        return;
    
    _private->visible = true;
    _private->visibilityAnimation.start(
        [this] { updateOpacity(); },
        0., 1., st::slideWrapDuration);
    
    _private->scrollArea->show();
    _private->closeButton->show();
    _private->refreshButton->show();
    
    updateLayout();
    emit visibilityChanged(true);
    
    BotLogger::info("BotCommands", "Commands panel shown");
}

void BotCommands::hide() {
    if (!_private->visible)
        return;
    
    _private->visible = false;
    _private->visibilityAnimation.start(
        [this] { 
            updateOpacity();
            if (!_private->visibilityAnimation.animating()) {
                _private->scrollArea->hide();
                _private->closeButton->hide();
                _private->refreshButton->hide();
            }
        },
        1., 0., st::slideWrapDuration);
    
    emit visibilityChanged(false);
    
    BotLogger::info("BotCommands", "Commands panel hidden");
}

bool BotCommands::isVisible() const {
    return _private->visible;
}

void BotCommands::setCommands(const MTPVector<MTPBotCommand> &commands) {
    BotLogger::info("BotCommands", QString("Setting %1 commands").arg(commands.v.size()));
    
    _private->commands.clear();
    _private->commands.reserve(commands.v.size());
    
    for (const auto &command : commands.v) {
        command.match([&](const MTPDbotCommand &data) {
            BotCommand cmd;
            cmd.command = qs(data.vcommand());
            cmd.description = qs(data.vdescription());
            _private->commands.append(cmd);
        });
    }
    
    _private->model->setCommands(_private->commands);
    
    // Clear height cache when commands change
    CommandHeightCache.clear();
}

void BotCommands::updateLayout() {
    const auto width = st::botCommandsWidth;
    const auto height = st::chatBotCommandsTop + st::botCommandsHeight;
    
    _private->scrollArea->resize(width, height - st::botCommandsButtonHeight - st::botCommandsCloseTop * 2);
    _private->scrollArea->moveToLeft(0, st::chatBotCommandsTop);
    
    _private->closeButton->moveToRight(
        st::botCommandsCloseRight,
        st::botCommandsCloseTop);
    
    _private->refreshButton->resize(width, st::botCommandsButtonHeight);
    _private->refreshButton->moveToLeft(
        0,
        height - st::botCommandsButtonHeight);
        
    updateOpacity();
}

void BotCommands::updateOpacity() {
    const auto opacity = _private->visibilityAnimation.value(_private->visible ? 1. : 0.);
    _private->scrollArea->setOpacity(opacity);
    _private->closeButton->setOpacity(opacity);
    _private->refreshButton->setOpacity(opacity);
}

} // namespace Chat 
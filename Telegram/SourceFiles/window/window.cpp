#include "window/window.h"
#include "window/controller.h"
#include "ui/widgets/fields/input_field.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "ui/effects/animations.h"
#include "ui/text/text_utilities.h"
#include "ui/icons.h"
#include "lang/lang_keys.h"
#include "main/main_session.h"

namespace Window {

Window::Window(not_null<Controller*> controller)
    : _controller(controller) {
    setupUI();
}

Window::~Window() = default;

void Window::setupUI() {
    _title.create(
        nullptr,
        tr::lng_app_name(tr::now),
        st::windowTitle);

    _menuButton.create(
        nullptr,
        st::windowMenuButton);

    _searchButton.create(
        nullptr,
        st::windowSearchButton);

    _settingsButton.create(
        nullptr,
        st::windowSettingsButton);

    _botMode = std::make_unique<BotMode>(_controller->session());

    connect(_menuButton, &Ui::IconButton::clicked, [=] {
        // TODO: Show menu
    });

    connect(_searchButton, &Ui::IconButton::clicked, [=] {
        // TODO: Show search
    });

    connect(_settingsButton, &Ui::IconButton::clicked, [=] {
        // TODO: Show settings
    });

    connect(_botMode.get(), &BotMode::visibilityChanged, [=](bool visible) {
        updateLayout();
        emit botModeChanged(visible);
    });
}

void Window::show() {
    if (_isVisible) return;
    _isVisible = true;
    _title->show();
    _menuButton->show();
    _searchButton->show();
    _settingsButton->show();
    updateLayout();
    emit visibilityChanged(true);
}

void Window::hide() {
    if (!_isVisible) return;
    _isVisible = false;
    _title->hide();
    _menuButton->hide();
    _searchButton->hide();
    _settingsButton->hide();
    if (_botMode) {
        _botMode->hide();
    }
    emit visibilityChanged(false);
}

bool Window::isVisible() const {
    return _isVisible;
}

void Window::setBotInfo(const MTPBotInfo &info) {
    if (_botMode) {
        _botMode->setRestrictions(info);
    }
}

void Window::updateLayout() {
    const auto top = st::windowTitleHeight;
    const auto left = st::windowTitleLeft;
    const auto right = st::windowTitleRight;

    _title->moveToLeft(left, top);
    _menuButton->moveToLeft(left + _title->width() + st::windowTitleSpacing, top);
    _searchButton->moveToRight(right + st::windowTitleSpacing, top);
    _settingsButton->moveToRight(right, top);

    if (_botMode && _botMode->isVisible()) {
        const auto botModeTop = top + st::windowTitleHeight + st::windowBotModeTop;
        _botMode->moveToLeft(left, botModeTop);
    }
}

} // namespace Window 
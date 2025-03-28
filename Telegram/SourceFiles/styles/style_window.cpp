#include "styles/style_window.h"
#include "ui/style/style_core.h"

namespace st {
namespace window {

const style::IconButton &botModeToggle() {
    static const auto result = style::IconButton{
        .width = 24,
        .height = 24,
        .icon = &st::windowBotModeIcon,
        .iconPosition = style::IconPosition::Left,
        .ripple = style::RippleAnimation::RoundRect,
        .rippleRadius = 12,
        .rippleColor = style::RippleColor::Default,
    };
    return result;
}

const style::RoundButton &botModeSettings() {
    static const auto result = style::RoundButton{
        .height = 28,
        .padding = style::margins(12, 0),
        .font = st::defaultButton.font,
        .textColor = st::defaultButton.textColor,
        .bgColor = st::defaultButton.bgColor,
        .bgColorOver = st::defaultButton.bgColorOver,
        .bgColorDown = st::defaultButton.bgColorDown,
        .bgColorDisabled = st::defaultButton.bgColorDisabled,
        .ripple = style::RippleAnimation::RoundRect,
        .rippleRadius = 14,
        .rippleColor = style::RippleColor::Default,
    };
    return result;
}

const style::IconButton &menuButton() {
    static const auto result = style::IconButton{
        .width = 24,
        .height = 24,
        .icon = &st::windowMenuIcon,
        .iconPosition = style::IconPosition::Left,
        .ripple = style::RippleAnimation::RoundRect,
        .rippleRadius = 12,
        .rippleColor = style::RippleColor::Default,
    };
    return result;
}

const style::IconButton &searchButton() {
    static const auto result = style::IconButton{
        .width = 24,
        .height = 24,
        .icon = &st::windowSearchIcon,
        .iconPosition = style::IconPosition::Left,
        .ripple = style::RippleAnimation::RoundRect,
        .rippleRadius = 12,
        .rippleColor = style::RippleColor::Default,
    };
    return result;
}

const style::IconButton &settingsButton() {
    static const auto result = style::IconButton{
        .width = 24,
        .height = 24,
        .icon = &st::windowSettingsIcon,
        .iconPosition = style::IconPosition::Left,
        .ripple = style::RippleAnimation::RoundRect,
        .rippleRadius = 12,
        .rippleColor = style::RippleColor::Default,
    };
    return result;
}

const style::FlatLabel &title() {
    static const auto result = style::FlatLabel{
        .font = st::windowTitleFont,
        .textColor = st::windowTitleColor,
        .textAlign = style::al_left,
        .lineHeight = st::windowTitleHeight,
    };
    return result;
}

} // namespace window
} // namespace st 
#include "styles/style_chat.h"
#include "ui/style/style_core.h"

namespace st {
namespace chat {

const style::FlatLabel &restrictionsLabel() {
    static const auto result = style::FlatLabel{
        .font = st::chatRestrictionsFont,
        .textColor = st::chatRestrictionsColor,
        .textAlign = style::al_left,
        .lineHeight = st::chatRestrictionsHeight,
    };
    return result;
}

const style::IconButton &restrictionsToggle() {
    static const auto result = style::IconButton{
        .width = 24,
        .height = 24,
        .icon = &st::chatRestrictionsIcon,
        .iconPosition = style::IconPosition::Left,
        .ripple = style::RippleAnimation::RoundRect,
        .rippleRadius = 12,
        .rippleColor = style::RippleColor::Default,
    };
    return result;
}

const style::FlatLabel &botCommandsTitle() {
    static const auto result = style::FlatLabel{
        .font = st::chatBotCommandsTitleFont,
        .textColor = st::chatBotCommandsTitleColor,
        .align = style::al_left,
        .lineHeight = st::chatBotCommandsTitleHeight,
    };
    return result;
}

const style::IconButton &botCommandsClose() {
    static const auto result = style::IconButton{
        .width = st::chatBotCommandsCloseSize,
        .height = st::chatBotCommandsCloseSize,
        .icon = st::chatBotCommandsCloseIcon,
        .iconPosition = style::IconPosition::Center,
        .ripple = style::RippleAnimation::Simple,
        .rippleRadius = st::chatBotCommandsCloseSize / 2,
        .rippleColor = st::chatBotCommandsCloseRippleColor,
    };
    return result;
}

const style::RoundButton &botCommandsButton() {
    static const auto result = style::RoundButton{
        .height = st::chatBotCommandsButtonHeight,
        .padding = st::chatBotCommandsButtonPadding,
        .font = st::chatBotCommandsButtonFont,
        .textColor = st::chatBotCommandsButtonTextColor,
        .bgColor = st::chatBotCommandsButtonBgColor,
        .bgColorHover = st::chatBotCommandsButtonBgColorHover,
        .bgColorActive = st::chatBotCommandsButtonBgColorActive,
        .bgColorDisabled = st::chatBotCommandsButtonBgColorDisabled,
        .ripple = style::RippleAnimation::Simple,
        .rippleRadius = st::chatBotCommandsButtonHeight / 2,
        .rippleColor = st::chatBotCommandsButtonRippleColor,
    };
    return result;
}

const style::FlatLabel &botSettingsTitle() {
    static const auto result = style::FlatLabel{
        .font = st::chatBotSettingsTitleFont,
        .textColor = st::chatBotSettingsTitleColor,
        .align = style::al_left,
        .lineHeight = st::chatBotSettingsTitleHeight,
    };
    return result;
}

const style::IconButton &botSettingsClose() {
    static const auto result = style::IconButton{
        .width = st::chatBotSettingsCloseSize,
        .height = st::chatBotSettingsCloseSize,
        .icon = st::chatBotSettingsCloseIcon,
        .iconPosition = style::IconPosition::Center,
        .ripple = style::RippleAnimation::Simple,
        .rippleRadius = st::chatBotSettingsCloseSize / 2,
        .rippleColor = st::chatBotSettingsCloseRippleColor,
    };
    return result;
}

const style::InputField &botSettingsDescription() {
    static const auto result = style::InputField{
        .height = st::chatBotSettingsDescriptionHeight,
        .padding = st::chatBotSettingsDescriptionPadding,
        .font = st::chatBotSettingsDescriptionFont,
        .textColor = st::chatBotSettingsDescriptionTextColor,
        .bgColor = st::chatBotSettingsDescriptionBgColor,
        .bgColorHover = st::chatBotSettingsDescriptionBgColorHover,
        .bgColorActive = st::chatBotSettingsDescriptionBgColorActive,
        .bgColorDisabled = st::chatBotSettingsDescriptionBgColorDisabled,
        .ripple = style::RippleAnimation::Simple,
        .rippleRadius = st::chatBotSettingsDescriptionHeight / 2,
        .rippleColor = st::chatBotSettingsDescriptionRippleColor,
    };
    return result;
}

const style::Checkbox &botSettingsCheckbox() {
    static const auto result = style::Checkbox{
        .height = st::chatBotSettingsCheckboxHeight,
        .padding = st::chatBotSettingsCheckboxPadding,
        .font = st::chatBotSettingsCheckboxFont,
        .textColor = st::chatBotSettingsCheckboxTextColor,
        .bgColor = st::chatBotSettingsCheckboxBgColor,
        .bgColorHover = st::chatBotSettingsCheckboxBgColorHover,
        .bgColorActive = st::chatBotSettingsCheckboxBgColorActive,
        .bgColorDisabled = st::chatBotSettingsCheckboxBgColorDisabled,
        .ripple = style::RippleAnimation::Simple,
        .rippleRadius = st::chatBotSettingsCheckboxHeight / 2,
        .rippleColor = st::chatBotSettingsCheckboxRippleColor,
    };
    return result;
}

const style::RoundButton &botSettingsSave() {
    static const auto result = style::RoundButton{
        .height = st::chatBotSettingsSaveHeight,
        .padding = st::chatBotSettingsSavePadding,
        .font = st::chatBotSettingsSaveFont,
        .textColor = st::chatBotSettingsSaveTextColor,
        .bgColor = st::chatBotSettingsSaveBgColor,
        .bgColorHover = st::chatBotSettingsSaveBgColorHover,
        .bgColorActive = st::chatBotSettingsSaveBgColorActive,
        .bgColorDisabled = st::chatBotSettingsSaveBgColorDisabled,
        .ripple = style::RippleAnimation::Simple,
        .rippleRadius = st::chatBotSettingsSaveHeight / 2,
        .rippleColor = st::chatBotSettingsSaveRippleColor,
    };
    return result;
}

const style::RoundButton &botSettingsCancel() {
    static const auto result = style::RoundButton{
        .height = st::chatBotSettingsCancelHeight,
        .padding = st::chatBotSettingsCancelPadding,
        .font = st::chatBotSettingsCancelFont,
        .textColor = st::chatBotSettingsCancelTextColor,
        .bgColor = st::chatBotSettingsCancelBgColor,
        .bgColorHover = st::chatBotSettingsCancelBgColorHover,
        .bgColorActive = st::chatBotSettingsCancelBgColorActive,
        .bgColorDisabled = st::chatBotSettingsCancelBgColorDisabled,
        .ripple = style::RippleAnimation::Simple,
        .rippleRadius = st::chatBotSettingsCancelHeight / 2,
        .rippleColor = st::chatBotSettingsCancelRippleColor,
    };
    return result;
}

} // namespace chat
} // namespace st 
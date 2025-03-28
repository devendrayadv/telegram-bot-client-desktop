#pragma once

#include "ui/style/style_core.h"

namespace st {
namespace chat {

constexpr auto chatTitleHeight = 48;
constexpr auto chatTitleLeft = 16;
constexpr auto chatTitleRight = 16;
constexpr auto chatTitleSpacing = 8;
constexpr auto chatInputHeight = 56;
constexpr auto chatRestrictionsTop = 8;

constexpr auto chatBotCommandsTop = 16;
constexpr auto chatBotCommandsLeft = 16;
constexpr auto chatBotCommandsRight = 16;
constexpr auto chatBotCommandsTitleHeight = 24;
constexpr auto chatBotCommandsSpacing = 8;

constexpr auto chatBotSettingsTop = 16;
constexpr auto chatBotSettingsLeft = 16;
constexpr auto chatBotSettingsRight = 16;
constexpr auto chatBotSettingsTitleHeight = 24;
constexpr auto chatBotSettingsSpacing = 8;
constexpr auto chatBotSettingsDescriptionHeight = 100;
constexpr auto chatBotSettingsCheckboxHeight = 24;

const style::FlatLabel &restrictionsLabel();
const style::IconButton &restrictionsToggle();
const style::FlatLabel &botCommandsTitle();
const style::IconButton &botCommandsClose();
const style::RoundButton &botCommandsButton();
const style::FlatLabel &botSettingsTitle();
const style::IconButton &botSettingsClose();
const style::InputField &botSettingsDescription();
const style::Checkbox &botSettingsCheckbox();
const style::RoundButton &botSettingsSave();
const style::RoundButton &botSettingsCancel();

} // namespace chat
} // namespace st 
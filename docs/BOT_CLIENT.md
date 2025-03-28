# Bot Client Mode for Kotatogram Desktop

This document describes the Bot Client Mode feature in Kotatogram Desktop, which allows users to manage and operate Telegram bots directly from the desktop client.

## Overview

Bot Client Mode transforms Kotatogram Desktop into a specialized interface for managing Telegram bots. This mode provides the following capabilities:

- Bot authentication using bot tokens
- Management of bot settings
- Handling bot commands
- Applying and managing chat restrictions
- Integration with Telegram Bot API

## Components

### 1. Bot Token Authentication

**Files:**
- `intro_bot_token.h`
- `intro_bot_token.cpp`

The Bot Token Authentication is the entry point to the Bot Client Mode. It allows users to authenticate using a bot token obtained from @BotFather. The token is validated and used to establish a session as the bot.

**Features:**
- Token validation
- Error handling
- Secure authentication
- User-friendly error messages

### 2. Bot Restrictions

**Files:**
- `chat_bot_restrictions.h`
- `chat_bot_restrictions.cpp`

Bot Restrictions manage what actions are allowed in chats based on the bot's capabilities and settings.

**Features:**
- Control sending messages
- Manage media uploads
- Handle stickers and GIFs permissions
- Inline mode controls
- UI for visualizing restrictions

### 3. Bot Commands

**Files:**
- `chat_bot_commands.h`
- `chat_bot_commands.cpp`
- `chat_bot_command_handler.h`
- `chat_bot_command_handler.cpp`

Bot Commands provide an interface for managing and executing bot commands.

**Features:**
- Command listing
- Command selection
- Command execution
- Response handling
- Error management

### 4. Bot Settings

**Files:**
- `chat_bot_settings.h`
- `chat_bot_settings.cpp`

Bot Settings allow users to configure and customize bot behavior and properties.

**Features:**
- Edit bot description
- Toggle inline mode
- Enable/disable group mode
- Control message reading capabilities
- Settings validation

### 5. Chat Widget Integration

**Files:**
- `chat_widget.h`
- `chat_widget.cpp`

The Chat Widget integrates all bot-related components into a cohesive interface.

**Features:**
- Unified UI for bot management
- Signal/slot connections between components
- Layout management
- Visibility control

## Styles and Language Support

**Styles:**
- `style_chat.h`
- `style_chat.cpp`

**Language:**
- `en.ts`

All UI elements follow Kotatogram Desktop's design language and support localization through language files.

## Usage

### Authentication

1. Launch Kotatogram Desktop
2. Navigate to the login screen
3. Select "Log in as Bot"
4. Enter the bot token from @BotFather
5. Click "Log In"

### Bot Management

Once authenticated as a bot, you can:

1. **View and edit bot settings:**
   - Click the settings button
   - Modify bot description
   - Toggle bot capabilities
   - Save changes

2. **Manage bot commands:**
   - Click the menu button
   - Select commands from the list
   - Execute commands
   - View command responses

3. **Interact with users:**
   - Respect chat restrictions
   - Respond to user messages
   - Send media and other content types

## Error Handling

The Bot Client includes comprehensive error handling for:
- Network issues
- Invalid tokens
- Server errors
- Validation failures
- Rate limiting

## Security Considerations

- Bot tokens are securely stored
- Commands are validated before execution
- Settings changes require confirmation
- API rate limits are respected

## Troubleshooting

### Common Issues

1. **Authentication Failure:**
   - Verify the bot token is correct
   - Check internet connection
   - Ensure the token hasn't been revoked

2. **Command Execution Failure:**
   - Check command syntax
   - Verify the bot has necessary permissions
   - Look for rate limiting issues

3. **Settings Not Saving:**
   - Ensure description length is valid
   - Check internet connection
   - Verify the bot owner is the current user

## API Integration

The Bot Client Mode integrates with the following Telegram Bot API endpoints:

- `/getMe` - Verify bot identity
- `/getUpdates` - Receive updates
- `/sendMessage` - Send messages
- `/getBotInfo` - Get bot information
- `/setBotInfo` - Update bot settings
- `/getBotCommands` - Get bot commands
- `/setBotCommands` - Update bot commands

## Performance Considerations

- Command lists are loaded efficiently
- UI updates are batched
- Network requests are optimized
- Resources are freed when not in use

## Future Improvements

- Integration with more Bot API features
- Enhanced analytics for bot usage
- More customization options
- Additional security features 
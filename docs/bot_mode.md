# Bot Mode Documentation

## Overview
Bot mode allows users to interact with Telegram as a bot using a bot token. This mode provides a simplified interface with specific restrictions and capabilities based on the bot's permissions.

## Features

### Authentication
- Users can authenticate using a bot token obtained from @BotFather
- The token is securely stored and managed by the session system
- Bot mode can be toggled on/off through the session management system

### Chat Interface
- Simplified chat interface optimized for bot interactions
- Visual indicators for bot capabilities and restrictions
- Disabled UI elements when actions are not allowed
- Clear feedback when actions are restricted

### Restrictions
- Message sending restrictions based on bot capabilities
- Media restrictions (photos, videos, files, etc.)
- Sticker and emoji restrictions
- Real-time updates of bot capabilities
- Visual feedback for restricted actions

## Implementation Details

### Session Management
The `Main::Session` class handles bot mode state:
- `isBotMode()`: Check if bot mode is active
- `setBotMode(bool)`: Enable/disable bot mode
- `botToken()`: Get current bot token
- `setBotToken(QString)`: Set new bot token

### Chat Widget
The `Chat::Widget` class integrates bot restrictions:
- `BotRestrictions` component for displaying restrictions
- Dynamic UI updates based on bot capabilities
- Proper layout management for restrictions display
- Signal/slot connections for restriction changes

### Styles
Bot mode specific styles are defined in:
- `style_chat.h`: Layout constants and component styles
- `style_chat.cpp`: Style implementations

## Usage

### Enabling Bot Mode
1. Obtain a bot token from @BotFather
2. Use the bot token authentication flow
3. Session will automatically enter bot mode

### Interacting with Chats
1. Chat interface adapts to bot capabilities
2. Restricted actions are visually disabled
3. Bot restrictions are displayed when relevant
4. Real-time updates reflect bot permission changes

## Security Considerations
- Bot tokens are stored securely
- Token validation on authentication
- Proper error handling for invalid/expired tokens
- Secure communication with Telegram servers

## Future Improvements
- Enhanced bot command support
- Custom bot menu integration
- Advanced bot settings management
- Improved error handling and user feedback 
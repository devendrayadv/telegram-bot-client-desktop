# Bot Client Mode - Developer Documentation

This document provides technical details for developers working on the Bot Client Mode in Kotatogram Desktop.

## Architecture Overview

The Bot Client Mode is built as a set of modular components that integrate with the existing Kotatogram Desktop codebase. The main components are:

1. **Bot Token Authentication** - Handles bot login flow
2. **Bot Restrictions** - Manages allowed actions in chat
3. **Bot Commands** - Provides command listing and execution
4. **Bot Settings** - Allows configuration of bot parameters
5. **Chat Widget Integration** - Combines all components into a unified interface

## Class Structure

```
├── Intro
│   └── BotTokenStep - Handles bot authentication
│
├── Chat
│   ├── Widget - Main chat interface with bot integration
│   ├── BotRestrictions - Manages bot capabilities in chat
│   ├── BotCommands - Displays and handles bot commands
│   ├── BotCommandHandler - Processes command execution
│   └── BotSettings - Configures bot parameters
│
└── BotLogger - Logging system for bot operations
```

## Component Details

### Bot Token Authentication

**Files:** `intro_bot_token.h`, `intro_bot_token.cpp`

The `BotTokenStep` class extends the intro flow to allow login via bot token. It validates the token format, sends requests to the Telegram API, and transitions to the main interface upon successful authentication.

Key methods:
- `validateToken(const QString &token)` - Checks token format
- `submit(const QString &token)` - Sends authentication request
- `showError(const QString &error)` - Displays validation errors

### Bot Restrictions

**Files:** `chat_bot_restrictions.h`, `chat_bot_restrictions.cpp`

The `BotRestrictions` class manages what actions are permitted in a chat based on the bot's capabilities. It automatically updates the UI to reflect these restrictions.

Key methods:
- `setRestrictions(const MTPBotInfo &info)` - Updates restriction state
- `canSendMessage()`, `canSendMedia()`, etc. - Check specific permissions
- `show()`, `hide()` - Manage visibility of restriction indicators

### Bot Commands

**Files:** `chat_bot_commands.h`, `chat_bot_commands.cpp`

The `BotCommands` class provides a virtualized list view of available commands, with performance optimizations for handling large command sets.

Key optimizations:
- Item height caching using `QCache`
- Lazy loading with batched rendering
- Pre-caching of visible items
- Virtual scrolling

### Bot Command Handler

**Files:** `chat_bot_command_handler.h`, `chat_bot_command_handler.cpp`

The `BotCommandHandler` class processes command execution requests, handles responses, and deals with error states.

Key methods:
- `handleCommand(const QString &command)` - Processes command execution
- `validateCommand(const QString &command)` - Checks command validity
- `sendCommand(const QString &command)` - Sends command to Telegram API

### Bot Settings

**Files:** `chat_bot_settings.h`, `chat_bot_settings.cpp`

The `BotSettings` class manages the bot's configuration options, including description, inline mode, and other parameters.

Key methods:
- `setBotInfo(const MTPBotInfo &info)` - Updates settings display
- `handleSave()` - Validates and saves settings
- `validateSettings()` - Checks settings validity

### Logging System

**Files:** `chat_bot_logger.h`, `chat_bot_logger.cpp`

The `BotLogger` namespace provides a thread-safe logging system for bot operations, with configurable log levels and output paths.

Key features:
- Multiple log levels (Debug, Info, Warning, Error, Critical)
- Thread-safe operation via QMutex
- Log rotation and file management
- Configurable minimum log level

## Performance Considerations

The Bot Client Mode includes several optimizations to ensure smooth performance:

1. **Command List Virtualization**
   - Virtual list model for efficient rendering
   - Height caching for complex items
   - Lazy loading of command data
   - Batch updates to reduce UI refreshes

2. **Network Optimization**
   - Rate limiting for API requests
   - Request batching where possible
   - Connection pooling

3. **Memory Management**
   - Efficient resource cleanup
   - Smart pointers for ownership management
   - Proper disposal of UI elements

4. **UI Responsiveness**
   - Background processing for expensive operations
   - Animation throttling during heavy operations
   - Deferred layout updates

## Testing

Unit tests are provided for critical components:

- `intro_bot_token_test.cpp` - Tests token validation and UI interaction
- Add more tests for each component as needed

## Adding New Features

When adding new features to the Bot Client Mode:

1. **Maintain Modularity**
   - Keep components separate and focused
   - Use signals/slots for communication between components

2. **Follow Performance Patterns**
   - Cache expensive calculations
   - Use lazy loading for large data sets
   - Batch network requests and UI updates

3. **Add Proper Logging**
   - Log important state changes
   - Include context information
   - Use appropriate log levels

4. **Update Documentation**
   - Add class descriptions
   - Document public API
   - Explain architectural decisions

5. **Write Tests**
   - Add unit tests for new functionality
   - Test edge cases and error conditions
   - Ensure backward compatibility

## Common Issues and Solutions

### UI Freezing with Large Command Sets

If the UI freezes when displaying many commands:
- Ensure the virtual list is properly implemented
- Verify that height caching is working
- Check that rendering is batched appropriately

### Memory Leaks

If memory usage grows over time:
- Check for proper cleanup of UI elements
- Verify smart pointer usage
- Ensure animations are stopped when components are hidden

### Network Errors

If network requests fail:
- Verify API endpoints are correct
- Check rate limit handling
- Ensure proper error reporting and recovery

## API Integration

The Bot Client Mode integrates with the following Telegram Bot API endpoints:

- `/getMe` - Basic bot information
- `/getUpdates` - Receive updates
- `/sendMessage` - Send text messages
- `/getBotInfo` - Bot configuration
- `/setBotInfo` - Update bot settings
- `/getBotCommands` - Get command list
- `/setBotCommands` - Update commands

Refer to the [Telegram Bot API documentation](https://core.telegram.org/bots/api) for detailed endpoint specifications. 
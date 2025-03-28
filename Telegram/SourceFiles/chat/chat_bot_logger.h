#pragma once

#include "base/basic_types.h"
#include <QString>

namespace BotLogger {

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error,
    Critical
};

// Initialize the bot logger
void init();

// Log a message with the specified level
void log(LogLevel level, const QString &component, const QString &message);

// Helper functions for different log levels
void debug(const QString &component, const QString &message);
void info(const QString &component, const QString &message);
void warning(const QString &component, const QString &message);
void error(const QString &component, const QString &message);
void critical(const QString &component, const QString &message);

// Enable/disable logging
void setEnabled(bool enabled);

// Set the minimum log level
void setMinLevel(LogLevel level);

// Set the log file path
void setLogFile(const QString &path);

// Get the current log file path
QString logFilePath();

// Flush log buffer to disk
void flush();

} // namespace BotLogger 
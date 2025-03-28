#include "chat/chat_bot_logger.h"

#include "base/platform/base_platform_file_utilities.h"
#include "base/platform/base_platform_info.h"
#include "core/application.h"
#include "core/core_settings.h"

#include <QDir>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QMutexLocker>

namespace {

QFile *LogFile = nullptr;
QTextStream *LogStream = nullptr;
QMutex LogMutex;
bool LoggingEnabled = false;
BotLogger::LogLevel MinLogLevel = BotLogger::LogLevel::Info;
QString CustomLogPath;

QString levelToString(BotLogger::LogLevel level) {
    switch (level) {
    case BotLogger::LogLevel::Debug:
        return "DEBUG";
    case BotLogger::LogLevel::Info:
        return "INFO";
    case BotLogger::LogLevel::Warning:
        return "WARNING";
    case BotLogger::LogLevel::Error:
        return "ERROR";
    case BotLogger::LogLevel::Critical:
        return "CRITICAL";
    default:
        return "UNKNOWN";
    }
}

QString getDefaultLogPath() {
    const auto basePath = QDir::currentPath();
    const auto fileName = QString("kotatogram_bot_%1.log").arg(
        QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    return QDir(basePath).filePath(fileName);
}

void ensureLogFileOpen() {
    if (LogFile && LogStream) {
        return;
    }

    const auto path = CustomLogPath.isEmpty()
        ? getDefaultLogPath()
        : CustomLogPath;

    LogFile = new QFile(path);
    if (!LogFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        delete LogFile;
        LogFile = nullptr;
        return;
    }

    LogStream = new QTextStream(LogFile);
}

void closeLogFile() {
    if (LogStream) {
        delete LogStream;
        LogStream = nullptr;
    }
    if (LogFile) {
        LogFile->close();
        delete LogFile;
        LogFile = nullptr;
    }
}

} // namespace

namespace BotLogger {

void init() {
    QMutexLocker lock(&LogMutex);
    LoggingEnabled = true;
    ensureLogFileOpen();
    
    if (LogStream) {
        *LogStream << "\n\n===== BOT CLIENT LOG STARTED AT "
                  << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                  << " =====\n\n";
        LogStream->flush();
    }
}

void log(LogLevel level, const QString &component, const QString &message) {
    if (!LoggingEnabled || level < MinLogLevel) {
        return;
    }

    QMutexLocker lock(&LogMutex);
    ensureLogFileOpen();
    
    if (!LogStream) {
        return;
    }

    const auto timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    const auto levelStr = levelToString(level);
    
    *LogStream << timestamp << " [" << levelStr << "] [" << component << "] " << message << "\n";
    LogStream->flush();
}

void debug(const QString &component, const QString &message) {
    log(LogLevel::Debug, component, message);
}

void info(const QString &component, const QString &message) {
    log(LogLevel::Info, component, message);
}

void warning(const QString &component, const QString &message) {
    log(LogLevel::Warning, component, message);
}

void error(const QString &component, const QString &message) {
    log(LogLevel::Error, component, message);
}

void critical(const QString &component, const QString &message) {
    log(LogLevel::Critical, component, message);
}

void setEnabled(bool enabled) {
    QMutexLocker lock(&LogMutex);
    LoggingEnabled = enabled;
    if (enabled) {
        ensureLogFileOpen();
    } else {
        closeLogFile();
    }
}

void setMinLevel(LogLevel level) {
    QMutexLocker lock(&LogMutex);
    MinLogLevel = level;
}

void setLogFile(const QString &path) {
    QMutexLocker lock(&LogMutex);
    CustomLogPath = path;
    
    closeLogFile();
    ensureLogFileOpen();
}

QString logFilePath() {
    QMutexLocker lock(&LogMutex);
    return CustomLogPath.isEmpty() ? getDefaultLogPath() : CustomLogPath;
}

void flush() {
    QMutexLocker lock(&LogMutex);
    if (LogStream) {
        LogStream->flush();
    }
}

} // namespace BotLogger 
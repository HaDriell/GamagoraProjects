#pragma once

#include "fmt/format.h"

enum LogLevel
{
    TRACE = 0,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

void LogImpl(LogLevel level, const char* format, fmt::format_args args);

template<typename... Args> void LogTrace(const char* format, const Args&... args)   { LogImpl(LogLevel::TRACE, format, fmt::make_format_args(args...)); }
template<typename... Args> void LogDebug(const char* format, const Args&... args)   { LogImpl(LogLevel::DEBUG, format, fmt::make_format_args(args...)); }
template<typename... Args> void LogInfo(const char* format, const Args&... args)    { LogImpl(LogLevel::INFO, format, fmt::make_format_args(args...)); }
template<typename... Args> void LogWarning(const char* format, const Args&... args) { LogImpl(LogLevel::WARNING, format, fmt::make_format_args(args...)); }
template<typename... Args> void LogError(const char* format, const Args&... args)   { LogImpl(LogLevel::ERROR, format, fmt::make_format_args(args...)); }
template<typename... Args> void LogFatal(const char* format, const Args&... args)   { LogImpl(LogLevel::FATAL, format, fmt::make_format_args(args...)); }

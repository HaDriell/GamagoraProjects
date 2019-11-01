#include "Log.h"

void LogImpl(LogLevel level, const char* format, fmt::format_args args)
{
    switch (level)
    {
    case LogLevel::TRACE  : fmt::print("[ TRACE ]"); break;
    case LogLevel::DEBUG  : fmt::print("[ DEBUG ]"); break;
    case LogLevel::INFO   : fmt::print("[ INFO  ]"); break;
    case LogLevel::WARNING: fmt::print("[WARNING]"); break;
    case LogLevel::ERROR  : fmt::print("[ ERROR ]"); break;
    case LogLevel::FATAL  : fmt::print("[ FATAL ]"); break;
    default               : fmt::print("[UNKNOWN]"); break;
    }
    fmt::vprint(format, args);
    fmt::print("\n");
}

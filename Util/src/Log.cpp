#include "Log.h"

#include <string>
#include <iostream>
#include <chrono>
#include "fmt/chrono.h"

void LogImpl(LogLevel level, const char* format, fmt::format_args args)
{
    //Fetch timestamp
    std::time_t t = std::time(nullptr);

    //Severity Prefix
    std::string_view severity;
    switch (level)
    {
        case LogLevel::TRACE  : severity = "[ TRACE ]"; break;
        case LogLevel::DEBUG  : severity = "[ DEBUG ]"; break;
        case LogLevel::INFO   : severity = "[ INFO  ]"; break;
        case LogLevel::WARNING: severity = "[WARNING]"; break;
        case LogLevel::ERROR  : severity = "[ ERROR ]"; break;
        case LogLevel::FATAL  : severity = "[ FATAL ]"; break;
        default               : severity = "[UNKNOWN]"; break;
    }
    std::cout << fmt::format("[{:%H:%M:%S}]", *std::localtime(&t)) << severity << fmt::vformat(format, args) << std::endl;
}

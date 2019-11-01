
#include "Log.h"


void test_log()
{
    LogTrace("This is a trace");
    LogDebug("This is a debug");
    LogInfo("This is an info");
    LogWarning("This is a warning");
    LogError("This is an error");
    LogFatal("This is fatal");
}
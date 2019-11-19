#pragma once

//Maths
#include "maths.h"
#include <limits>

//IO ; FS
#include <iostream>
#include <fstream>
#include <VFS.h>
#include <Assets.h>

//String integration
#include <string>
#include <cstring>
#include <sstream>

//Util integration
#include "Timer.h"
#include <functional>

//Collections
#include <vector>
#include <unordered_map>

//Logging
#include "Log.h"


//Assertions
#include <signal.h>
#ifdef _DEBUG
    #define GL_ASSERT(x, ...) { if (!(x)) { LogError("Assertion Failed: {0}", __VA_ARGS__); raise(SIGTRAP); } }
#else
    #define GL_ASSERT(x, ...)
#endif


//Memory management
#include <memory>
template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename... Args>
constexpr Ref<T> MakeRef(Args&& ... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using Scope = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Scope<T> MakeScope(Args&& ... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}
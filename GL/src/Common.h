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
template<typename T>
const auto MakeRef = std::make_shared<T>;

template<typename T>
using Scope = std::unique_ptr<T>;
template<typename T>
const auto MakeScope = std::make_unique<T>;
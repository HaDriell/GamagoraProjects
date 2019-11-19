#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>
#include <stdint.h>
#include <memory>

struct ProfilerData
{
    std::string name;
    uint32_t    tid;
    uint64_t    begin;
    uint64_t    end;
};

class Profiler
{
private:
    std::ofstream   stream;
    bool            empty;

public:
    Profiler(const std::string& filepath) : 
    stream(filepath), 
    empty(true)
    {
        writeHeader();
    }

    ~Profiler()
    {
        writeFooter();
        stream.close();
    }

    void writeHeader()
    {
        stream << "{\"otherData\": {}, \"traceEvents\":[";
        stream.flush();
    }

    void writeProfile(const ProfilerData& data)
    {
        if (!empty) stream << ",";
        else empty = false;
        
        // avoid breaking the JSON file with double quotes in names
        std::string name = data.name;
        std::replace(name.begin(), name.end(), '"', '\'');

        stream << "{";
        stream << "\"cat\":\"function\",";
        stream << "\"name\":\"" << name << "\",";
        stream << "\"tid\":"<< data.tid << ",";
        stream << "\"ts\":" << data.begin << ",";
        stream << "\"dur\":" << (data.end - data.begin) << ",";
        stream << "\"ph\":\"X\",";
        stream << "\"pid\":0";
        stream << "}";

        stream.flush();
    }

    void writeFooter()
    {
        stream << "]}";
        stream.flush();
    }

    //Unique Profiler Session Manager
private:
    static std::unique_ptr<Profiler> session;
public:

    static void Begin(const std::string& filepath = "result.json")
    {
        session = std::make_unique<Profiler>(filepath);
    }

    static std::unique_ptr<Profiler>& Get()
    {
        if (!session) 
            Begin();
        return session;
    }


    static void End()
    {
        session.reset();
    }
};
std::unique_ptr<Profiler> Profiler::session = nullptr;

class ProfilerTimer
{
private:
    using Clock     = std::chrono::high_resolution_clock;
    using Timestamp = Clock::time_point;

    std::string name;
    Timestamp   beginTimestamp;
    bool        stopped;

public:
    ProfilerTimer(const std::string& name) : 
    name(name), 
    beginTimestamp(Clock::now()), 
    stopped(false) 
    { }

    ~ProfilerTimer()
    {
        if (!stopped) stop();
    }

    void stop()
    {
        Timestamp endTimestamp = Clock::now();
        uint64_t begin = std::chrono::time_point_cast<std::chrono::microseconds>(beginTimestamp).time_since_epoch().count();
        uint64_t end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimestamp).time_since_epoch().count();
        uint32_t tid = std::hash<std::thread::id>{}(std::this_thread::get_id());
        Profiler::Get()->writeProfile(ProfilerData{name, tid, begin, end});
    }
};

#ifdef PROFILING
    #define PROFILE_SCOPE(name) ProfilerTimer timer##__LINE__(name)
    #define PROFILE_FUNCTION()  PROFILE_SCOPE(__PRETTY_FUNCTION__)
#else
    #define PROFILE_SCOPE(name)
    #define PROFILE_FUNCTION()
#endif
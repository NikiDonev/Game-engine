//
// Basic instrumentation profiler by Cherno

// Usage: include this header file somewhere in your code (eg. precompiled header), and then use like:
//
// Instrumentor::Get().BeginSession("Session Name");        // Begin session 
// {
//     InstrumentationTimer timer("Profiled Scope Name");   // Place code like this in scopes you'd like to include in profiling
//     // Code
// }
// Instrumentor::Get().EndSession();                        // End Session
//
// You will probably want to macro-fy this, to switch on/off easily and use things like __FUNCSIG__ for the profile name.
//



#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <regex>
#include <thread>
#include <mutex>


#define PROFILING 1
#if PROFILING
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
    #define PROFILE_SESSION(filepath) Instrumentor::BeginSession(filepath)
    #if defined(_MSC_VER)
    #define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
    #elif defined(__GNUC__) || defined(__clang__)
    #define PROFILE_FUNCTION() PROFILE_SCOPE(__PRETTY_FUNCTION__)
    #else
    #define PROFILE_FUNCTION() PROFILE_SCOPE(__func__)
    #endif
#else
#define PROFILE_SESSION(filepath) 
#define PROFILING_SCOPE(name)
#define PROFILE_FUNCTION()
#endif


struct ProfileResult
{
    std::string Name;
    long long Start, End;
    uint32_t ThreadID;
};


class Instrumentor
{
public:
    static void BeginSession(const std::string& filepath = "results.json")
    {
        if (m_ActiveSession) EndSession();
        m_ActiveSession = true;
        m_OutputStream.open(filepath);
        WriteHeader();
    }

    static void EndSession()
    {
        if (!m_ActiveSession) return;
        m_ActiveSession = false;
        WriteFooter();
        m_OutputStream.close();
        m_ProfileCount = 0;
    }

    static void WriteProfile(const ProfileResult& result)
    {
        std::lock_guard<std::mutex> lock(m_Lock);

        if (m_ProfileCount++ > 0) m_OutputStream << ",";

        std::string name = result.Name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_OutputStream << "{";
        m_OutputStream << "\"cat\":\"function\",";
        m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
        m_OutputStream << "\"name\":\"" << name << "\",";
        m_OutputStream << "\"ph\":\"X\",";
        m_OutputStream << "\"pid\":0,";
        m_OutputStream << "\"tid\":" << result.ThreadID << ",";
        m_OutputStream << "\"ts\":" << result.Start;
        m_OutputStream << "}";

        m_OutputStream.flush();
    }

    static void WriteHeader()
    {
        m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
    }

    static void WriteFooter()
    {
        m_OutputStream << "]}";
    }

private:
    static inline bool m_ActiveSession{ false };
    static inline std::ofstream m_OutputStream;
    static inline int m_ProfileCount{ 0 };
    static inline std::mutex m_Lock;
};


class InstrumentationTimer
{
public:
    InstrumentationTimer(const char* name)
        : m_Name(name), m_Stopped(false)
    {
        if (auto p = m_Name.find("__cdecl "); p != std::string::npos) m_Name.erase(p, 8);
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~InstrumentationTimer()
    {
        if (!m_Stopped)
            Stop();
    }

    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
        Instrumentor::WriteProfile({ m_Name, start, end, threadID });

        m_Stopped = true;
    }
private:
    std::string m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    bool m_Stopped;
};



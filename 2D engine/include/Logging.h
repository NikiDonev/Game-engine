#pragma once
#include <vector>
#include <string>
#include <chrono>
#include <cstdarg>
#include <cstdio>

#include <imgui.h>

enum class LogLevel : uint8_t {
	Info,
	Warning,
	Error
};

struct LogEntry {
	LogLevel level;
	float time;
	std::string text;
};

class DebugLog
{
public:
	LogLevel logLevel = LogLevel::Info;
	std::vector<LogEntry> logs;

	DebugLog(uint16_t MaxLogs = 200) : m_MaxLogs(MaxLogs) { logs.reserve(m_MaxLogs); }
	
	void Add(LogLevel level, const std::string& message);
	void Add(LogLevel level, const char* fmt, ...);
	void DrawLog();
	uint16_t getMaxLogs() { return m_MaxLogs; };
	void setMaxLogs(uint16_t maxLogs) { 
		m_MaxLogs = maxLogs; 
		logs.clear();
		logs.reserve(m_MaxLogs);
		m_HeadI = 0;
	}

private:
	void PushLog(LogLevel level, float time, const std::string& message);
	float getTime();
	void DrawMessage(const LogEntry& entry);

	uint16_t m_HeadI{0};
	uint16_t m_MaxLogs = 200;
	bool m_Paused = false;
}; 

inline DebugLog debugLog;


#if PRODUCTION_BUILD
	#define LOG_INFO(...) ((void)0)
	#define LOG_WARN(...) ((void)0)
	#define LOG_ERROR(...) ((void)0)
	#define DRAW_LOG() ((void)0)
#else
	#define LOG_INFO(...) debugLog.Add(LogLevel::Info, __VA_ARGS__);
	#define LOG_WARN(...) debugLog.Add(LogLevel::Warning, __VA_ARGS__);
	#define LOG_ERROR(...) debugLog.Add(LogLevel::Error, __VA_ARGS__);
	#define DRAW_LOG() debugLog.DrawLog()
#endif
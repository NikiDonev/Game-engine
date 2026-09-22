#include "Logging.h"

void DebugLog::Add(LogLevel level, const std::string& message) {
	float time = getTime();
	PushLog(level, time, message);
}

void DebugLog::Add(LogLevel level, const char* fmt, ...)
{
	float time = getTime();
	char buffer[512];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	PushLog(level, time, buffer);
}

void DebugLog::DrawLog() {
	ImGui::Begin("Log");

	if (ImGui::Button("Clear")) logs.clear();

	ImGui::SameLine();
	if (ImGui::Button(m_Paused ? "Unpause" : "Pause")) m_Paused = !m_Paused;

	ImGui::SameLine();
	ImGui::SetNextItemWidth(120);
	ImGui::PushID(1);
	int current = static_cast<int>(logLevel);
	if (ImGui::Combo("", &current, "Info\0Warning\0Error\0"))
		logLevel = static_cast<LogLevel>(current);
	ImGui::PopID();

	ImGui::SameLine();
	ImGui::SetNextItemWidth(80);
	int MaxLogs = static_cast<int>(m_MaxLogs);
	if (ImGui::DragInt("Log size", &MaxLogs, 1.0f, 1, 1000))
		setMaxLogs(static_cast<uint16_t>(MaxLogs));




	ImGui::Separator();

	ImGui::BeginChild("LogScroll", ImVec2(0, 0), false,
		ImGuiWindowFlags_HorizontalScrollbar);
	if (logs.size() < m_MaxLogs) {
		for (int i = 0; i < logs.size(); ++i)
			DrawMessage(logs[i]);
	}
	else {
		for (int i = m_HeadI; i < logs.size(); ++i)
			DrawMessage(logs[i]);
		for (int i = 0; i < m_HeadI; ++i)
			DrawMessage(logs[i]);
	}

	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);
	ImGui::EndChild();
	ImGui::End();
}

void DebugLog::PushLog(LogLevel level, float time, const std::string& message) {
	if (level < logLevel) return;
	if (m_Paused) return;
	if (logs.size() < m_MaxLogs) {
		logs.push_back({ level, time, message });
		m_HeadI++;
	}
	else {
		logs[m_HeadI] = { level, time, message };
		m_HeadI++;
	}
	if (m_HeadI >= m_MaxLogs) m_HeadI = 0;
}

float DebugLog::getTime() {
	using Clock = std::chrono::steady_clock;
	static const auto start = Clock::now();
	auto now = Clock::now();
	return std::chrono::duration<float>(now - start).count();
}

void DebugLog::DrawMessage(const LogEntry& entry) {
	ImVec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
	std::string levelStr;
	switch (entry.level) {
	case LogLevel::Info:
		color = { 1.0f, 1.0f, 1.0f, 1.0f };
		levelStr = "[INFO]   ";
		break;
	case LogLevel::Warning:
		color = { 1.0f, 1.0f, 0.0f, 1.0f };
		levelStr = "[WARNING]";
		break;
	case LogLevel::Error:
		color = { 1.0f, 0.0f, 0.0f, 1.0f };
		levelStr = "[ERROR]  ";
		break;
	}
	ImGui::TextColored(color, "%s %.3fs:  %s", levelStr.c_str(), entry.time, entry.text.c_str());
}
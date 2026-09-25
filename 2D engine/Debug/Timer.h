#pragma once
#include <vector>
#include <chrono>
#include <string>
#include <utility>

struct Timer {
public:
	std::chrono::high_resolution_clock::time_point start{};
	std::string timerData;
	std::vector<std::pair<std::string, float>> timePoints;

	void TimePoint(std::string name) {
		auto now = std::chrono::high_resolution_clock::now();
		if (timePoints.empty()) {
			timePoints.push_back({ name, 0.0f });
			return;
		}

		float ms = std::chrono::duration<float, std::milli>(now - start).count();
		start = now;

		timePoints.back().second = ms;
		timePoints.push_back({ name, 0.0f });
	}
	void Reset() {
		TimePoint("End");
		timePoints.pop_back();

		timerData.clear();
		for (const auto& [name, ms] : timePoints) {
			std::string newLine = (name + ": " + std::to_string(ms) + "\n");
			timerData += newLine;
		}
		timePoints.clear();
	}

private:
};
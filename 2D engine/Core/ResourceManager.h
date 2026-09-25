#pragma once
#include <memory>
#include <unordered_map>
#include <functional>
#include <sstream>
#include <utility>

template <typename T>
using Ref = std::shared_ptr<T>;


class ResourceManager {
public:
	ResourceManager() {

	}
	template <typename T, typename... Args>
	Ref<T> Load(const std::string& path, Args&&... args) {
		std::string key = CreateKey(path, args...);
		auto& cache = Cache<T>::s_Map;
		auto it = cache.find(key);
		if (it != cache.end()) {
			if (auto ref = it->second.lock()) 
				return ref;
		}

		Ref<T> ref = std::make_shared<T>(path, std::forward<Args>(args)...);
		cache[key] = std::weak_ptr<T>(ref);
		return ref;
	}

private:
	template <typename... Args>
	std::string CreateKey(const std::string& path, Args&&... args) {
		std::stringstream ss;
		ss << path;
		((ss << "|" << args), ...); // Fold expression - puts all the arguments into one string used as a key
		return ss.str();
	}
	template <typename T>
	struct Cache { 
		static std::unordered_map<std::string, std::weak_ptr<T>>s_Map; 
	};
};

template <typename T> // Static cache for all ResourceManager instances
std::unordered_map<std::string, std::weak_ptr<T>> ResourceManager::Cache<T>::s_Map;
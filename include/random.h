#pragma once
#include <iostream>
#include <cstdlib>
#include <type_traits>
#include <chrono>



	class RNG {
	private:
		uint64_t state{};
		uint64_t seed{};
	public:
		RNG(uint64_t s = 0) : seed(s) {
			if (seed == 0) {
				seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
			}
			state = seed;
		}
		uint64_t random_data() {
			uint64_t x = state;
			x ^= x << 13;
			x ^= x >> 7;
			x ^= x << 17;
			return state = x;
		}
		template <typename T>
		T number(T min, T max) {
			if constexpr (std::is_integral_v<T>) {
				uint64_t range = static_cast<uint64_t>(max - min + 1);
				return min + static_cast<T>(random_data() % range);
			}
			else if constexpr (std::is_floating_point_v<T>) {
				double normalized = static_cast<double>(random_data()) / UINT64_MAX;
				return min + static_cast<T>(normalized * (max - min));
			}
		}
	};
	inline RNG rng;


	bool random_event(double probability, size_t throws = 1) {
		if (throws == 0) return false;
		probability = std::clamp(probability, 0.0, 1.0);
		return rng.number(0.0, 1.0) > std::pow(1 - probability, throws);
	}
	template <typename T>
	T picker(const std::vector<T>& elements, const std::vector<double>& weight) {
		double total_weight = 0.0;
		int n = weight.size();
		for (int i = 0; i < elements.size(); ++i) total_weight += weight[i % n];
		double item = rng.number(0.0, total_weight);
		for (int i = 0; i < elements.size(); ++i) {
			total_weight -= weight[i % n];
			if (total_weight < item) return elements[i];
		}
	}


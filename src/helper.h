#pragma once
#include <glm/glm.hpp>
#include <chrono>
#include <random>
glm::vec3 RandomRotation() {
	auto rot = glm::vec3(0);
	std::default_random_engine generator;// std::chrono::system_clock::now());
	std::uniform_real_distribution<float> dx(-360, 360);
	std::uniform_real_distribution<float> dz(-360, 360);
	std::uniform_real_distribution<float> dy(-360, 360);

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	generator.seed(seed);
	rot.x = dx(generator);
	rot.y = dy(generator);
	rot.z = dz(generator);

	return rot;
}
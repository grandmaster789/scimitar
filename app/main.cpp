#include <iostream>
#include <chrono>
#include <thread>

#include <scimitar.h>
#include <vulkan/vulkan.hpp>

#include <deque>

int main() {
	std::cout << "Launching application\n";

	{
		auto& engine = scimitar::core::Engine::instance();
		auto window = engine.createWindow();

		std::deque<std::unique_ptr<int>> x;
		x.push_back(std::make_unique<int>(6));

		auto local = scimitar::util::create_non_owning_copy(x);

		engine.start();
	}
	
	std::cout << "Application closed\n";
}
//
// Created by tomsm on 29/05/2022.
//

#include <vulkan_application.h>
#include <spdlog/spdlog.h>
#include <stdexcept>

int main()
{

	try {
		vulkan_application vulkan_app;
		vulkan_app.run();
	} catch (const std::runtime_error& e) {
		spdlog::critical(e.what());

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
//
// Created by Tom Smale on 30/05/2022.
//

#include "vulkan_application.h"
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <vector>

vulkan_application::vulkan_application()
{
	const char *app_name = "vulkan-thingy";

	// init glfw
	if (glfwInit() == GLFW_FALSE) {
		throw std::runtime_error("failed to initialize GLFW");
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	_win = glfwCreateWindow(WIN_W, WIN_H, app_name, nullptr, nullptr);

	// init vulkan
	VkApplicationInfo app_info{};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = app_name;
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = nullptr;
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;

	// vulkan extensions
	uint32_t ext_count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, nullptr);
	std::vector<VkExtensionProperties> exts(ext_count);
	vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, exts.data());
	spdlog::info("available vulkan extensions:");
	for (const auto &ext : exts)
		fmt::print("\t{}\n", ext.extensionName);
	uint32_t glfw_ext_count = 0;
	const char** glfw_exts;
	glfw_exts = glfwGetRequiredInstanceExtensions(&glfw_ext_count);

	create_info.enabledExtensionCount = glfw_ext_count;
	create_info.ppEnabledExtensionNames = glfw_exts;

	// vulkan validation layers
	create_info.enabledLayerCount = 0;

	if (vkCreateInstance(&create_info, nullptr, &_inst) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance");
	}
}

vulkan_application::~vulkan_application()
{
	vkDestroyInstance(_inst, nullptr);

	glfwDestroyWindow(_win);
	glfwTerminate();
}

void vulkan_application::run()
{
	while (!glfwWindowShouldClose(_win)) {
		glfwPollEvents();



		glfwSwapBuffers(_win);
	}
}
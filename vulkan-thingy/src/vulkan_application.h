//
// Created by Tom Smale on 30/05/2022.
//

#ifndef VULKAN_THINGY_VULKAN_APPLICATION_H
#define VULKAN_THINGY_VULKAN_APPLICATION_H

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

class vulkan_application {
public:
	vulkan_application();
	~vulkan_application();
	void run();
private:
	// glfw
	static constexpr int WIN_H = 480;
	static constexpr int WIN_W = 640;
	GLFWwindow* _win;

	// vulkan instance
	VkInstance _inst;
};

#endif //VULKAN_THINGY_VULKAN_APPLICATION_H

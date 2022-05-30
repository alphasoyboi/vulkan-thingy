//
// Created by Tom Smale on 30/05/2022.
//

#ifndef VULKAN_THINGY_VULKAN_APPLICATION_H
#define VULKAN_THINGY_VULKAN_APPLICATION_H

#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vector>

class vulkan_application {
public:
	vulkan_application();
	~vulkan_application();
	void run();
private:
	// glfw
	static constexpr int _win_w = 640;
    static constexpr int _win_h = 480;
	GLFWwindow* _win;

	// vulkan instance
	VkInstance _inst;
    VkDebugUtilsMessengerEXT _debug_messenger;

    static void                           print_available_extensions();
    static std::vector<const char *>      get_required_extensions();
    static bool                           check_validation_layer_support();
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT             messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                        void                                       *pUserData);
};

#endif // VULKAN_THINGY_VULKAN_APPLICATION_H

//
// Created by Tom Smale on 30/05/2022.
//

#include "vulkan_application.h"
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace {
#if defined(NDEBUG)
    constexpr bool s_debug_features = false;
#else
    constexpr bool s_debug_features = true;
#endif
    const char *s_app_name = "vulkan-thingy";

    const std::vector<const char *> s_validation_layers = {
            "VK_LAYER_KHRONOS_validation"
    };
}



vulkan_application::vulkan_application()
{
    if (s_debug_features)
        spdlog::set_level(spdlog::level::trace);
    else
        spdlog::set_level(spdlog::level::err);


        // INIT GLFW //
	if (glfwInit() == GLFW_FALSE)
		throw std::runtime_error("failed to initialize GLFW");

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	_win = glfwCreateWindow(_win_w, _win_h, s_app_name, nullptr, nullptr);

        // INIT VULKAN //
	VkApplicationInfo app_info{};
	app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName   = s_app_name;
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName        = nullptr;
	app_info.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
	app_info.apiVersion         = VK_API_VERSION_1_0;

	VkInstanceCreateInfo create_info{};
	create_info.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;

        // VULKAN EXTENSIONS //
    // print extensions
    if (s_debug_features)
        print_available_extensions();

    // get required extensions
    auto exts = get_required_extensions();
    create_info.enabledExtensionCount   = exts.size();
    create_info.ppEnabledExtensionNames = exts.data();

	    // VULKAN VALIDATION LAYERS
    if (s_debug_features) {
        create_info.enabledLayerCount   = s_validation_layers.size();
        create_info.ppEnabledLayerNames = s_validation_layers.data();
    } else {
        create_info.enabledLayerCount   = 0;
    }

	if (vkCreateInstance(&create_info, nullptr, &_inst) != VK_SUCCESS)
		throw std::runtime_error("failed to create instance");
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

void vulkan_application::print_available_extensions()
{
    uint32_t ext_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, nullptr);
    std::vector<VkExtensionProperties> exts(ext_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, exts.data());

    spdlog::info("available vulkan extensions:");
    for (const auto &ext : exts)
        fmt::print("\t{}\n", ext.extensionName);
}

std::vector<const char *> vulkan_application::get_required_extensions()
{
    uint32_t glfw_ext_count = 0;
    const char** glfw_exts;
    glfw_exts = glfwGetRequiredInstanceExtensions(&glfw_ext_count);
    std::vector<const char *> exts(glfw_exts, glfw_exts + glfw_ext_count);

    if (s_debug_features)
        exts.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return exts;
}

bool vulkan_application::check_validation_layer_support()
{
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    for (const char* layer_name : s_validation_layers) {
        bool layer_found = false;

        for (const auto& layer_properties : available_layers) {
            if (strcmp(layer_name, layer_properties.layerName) == 0) {
                layer_found = true;
                break;
            }
        }

        if (!layer_found)
            return false;
    }

    return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_application::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
                                                                 VkDebugUtilsMessageTypeFlagsEXT             messageType,
                                                                 const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                                 void                                       *pUserData)
{
    if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        spdlog::warn("validation layer: {}\n", pCallbackData->pMessage);
    else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        spdlog::error("validation layer: {}\n", pCallbackData->pMessage);

    return VK_FALSE;
}
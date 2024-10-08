// #include "engine.hpp"

#if defined(WINDOWS)
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#elif defined(LINUX) || defined(OSX)
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
#else
    #error Unknown dynamic link import/export semantics.
#endif

#include <stdio.h>          // printf, fprintf
#include <stdlib.h>         // abort
#include <vulkan/vulkan.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

VkBool32 debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                        VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    auto types = vk::to_string((vk::DebugUtilsMessageTypeFlagsEXT)messageTypes);
    auto severity = vk::to_string((vk::DebugUtilsMessageSeverityFlagBitsEXT)messageSeverity);
    printf("%s%s: %s\n", severity.c_str(), types.c_str(), pCallbackData->pMessage);
    return VK_FALSE;
}

// Exporting function to calling from launcher (ak executable binary)
extern "C" {
    EXPORT int engineMain(int argc, char ** argv) {
	    // Engine *engine = new Engine();
	    // engine->init(argc, argv);
	    // engine->init_gui();
	    // engine->run();

        SDL_Init(SDL_INIT_VIDEO);
        auto wpos = SDL_WINDOWPOS_CENTERED;
        auto win = SDL_CreateWindow("vulkantest", wpos, wpos, 800, 600, SDL_WINDOW_VULKAN);

        vk::InstanceCreateInfo createInfo;

        SDL_Vulkan_GetInstanceExtensions(win, &createInfo.enabledExtensionCount, nullptr);
        std::vector<const char*> extensions(createInfo.enabledExtensionCount);
        SDL_Vulkan_GetInstanceExtensions(win, &createInfo.enabledExtensionCount, extensions.data());

        extensions.push_back("VK_EXT_debug_utils");
        extensions.push_back("VK_EXT_debug_report");
        createInfo.enabledExtensionCount = extensions.size();
        createInfo.ppEnabledExtensionNames = extensions.data();

        createInfo.enabledLayerCount = 1;
        const char* validationLayers[] = {"VK_LAYER_KHRONOS_validation"};
        createInfo.ppEnabledLayerNames = validationLayers;
	    return 0;
    }
}
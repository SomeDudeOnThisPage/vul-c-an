#include "vulkan.h"

#ifdef DEBUG
    uint32_t requested_layer_count = 1;
    const char* requested_layers[] = {
        "VK_LAYER_KHRONOS_validation"
    };
#else
    uint32_t requested_layer_count = 0;
    const char* requested_layers[0];
#endif

void vulkan_destroy(VulkanInstance_t *instance) {
    VkDevice vk_device = *instance->logical_device;
    DEBUG_PRINT("destroying device '%p' %p\n", instance->logical_device, vk_device);
    vkDestroyDevice(vk_device, NULL);

    // this operation also destroys the physical device
    DEBUG_PRINT("destroying instance '%p'\n", instance->instance);
    VkInstance vk_instance = *instance->instance;
    vkDestroyInstance(vk_instance, NULL);
}

uint32_t query_extension_count() {
    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &extension_count, NULL);
    return extension_count;
}

uint32_t query_validation_layer_count() {
    uint32_t validation_layer_count;
    vkEnumerateInstanceLayerProperties(&validation_layer_count, NULL);
    return validation_layer_count;
}


void enumerate_extensions(uint32_t extension_count, VkExtensionProperties *extensions) {
    // query the info of available extensions
    vkEnumerateInstanceExtensionProperties(NULL, &extension_count, extensions);
    DEBUG_PRINT("Queried %d extensions:\n", extension_count);
#ifdef DEBUG
    for (uint32_t i = 0; i < extension_count; i++) {
        DEBUG_PRINT("%d:\t%s\n", i, extensions[i].extensionName);
    }
#endif
}

void enumerate_validation_layers(uint32_t validation_layer_count, VkLayerProperties *properties) {
    vkEnumerateInstanceLayerProperties(&validation_layer_count, properties);
    DEBUG_PRINT("Queried %d validation layers:\n", validation_layer_count);
#ifdef DEBUG
    for (uint32_t i = 0; i < validation_layer_count; i++) {
        DEBUG_PRINT("%d:\t%s\n", i, properties[i].layerName);
    }
#endif
}

uint8_t check_glfw_extensions(const uint32_t *glfw_extension_count, const char **glfw_extensions, uint32_t extension_count, VkExtensionProperties *extensions) {
    DEBUG_PRINT("GLFW requires the following %d extensions:\n", *glfw_extension_count);

    for (uint32_t i = 0; i < *glfw_extension_count; i++) {
        // check if we support all extensions required by glfw
        uint8_t supported = 0;
        // check if our extensions support the required glfw extension
        for (uint32_t j = 0; j < extension_count; j++) {
            if (strcmp(glfw_extensions[i], extensions[j].extensionName) == 0) {
                supported = 1;
                break;
            }
        }

        DEBUG_PRINT("%d:\t%s\t%s", i, glfw_extensions[i], supported == 1 ? "SUPPORTED\n" : "NOT SUPPORTED\n");

        if (!supported) {
            return 0;
        }
    }

    return 1;
}

uint8_t check_validation_layers(uint32_t layer_count, VkLayerProperties* layers) {
    DEBUG_PRINT("Requested the following %d validation layers:\n", requested_layer_count);

    for (uint32_t i = 0; i < requested_layer_count; i++) {
        uint8_t supported = 0;

        for (uint32_t j = 0; j < layer_count; j++) {
            if (strcmp(requested_layers[i], layers[j].layerName) == 0) {
                supported = 1;
                break;
            }
        }

        DEBUG_PRINT("%d:\t%s\t%s", i, requested_layers[i], supported == 1 ? "SUPPORTED\n" : "NOT SUPPORTED\n");

        if (!supported) {
            return 0;
        }
    }
    return 1;
}

uint8_t vulkan_initialize_queue_families(VulkanInstance_t *vulkan_instance) {
    VkPhysicalDevice physical_device = *vulkan_instance->physical_devices[vulkan_instance->physical_device];

    uint32_t queue_family_count;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, NULL);

    printf("QueueFamilyCount %d\n", queue_family_count);

    VkQueueFamilyProperties properties[queue_family_count];
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, properties);

    for (uint32_t i = 0; i < queue_family_count; i++) {
        // TODO: define this somewhere in a config, add support for multiple different flags in different queues
        if (properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            vulkan_instance->graphics_queue_family = i;
            return 1;
        }
    }

    return 0;
}

uint8_t vulkan_initialize_physical_device(VulkanInstance_t *vulkan_instance) {
    vulkan_instance->physical_device = 0;

    uint32_t physical_device_count;
    vkEnumeratePhysicalDevices(*vulkan_instance->instance, &physical_device_count, NULL);

    DEBUG_PRINT("Queried %d physical devices supporting Vulkan\n", physical_device_count);
    if (physical_device_count <= 0) {
        return 0;
    }

    VkPhysicalDevice *devices = malloc(sizeof(VkPhysicalDevice) * physical_device_count);
    vkEnumeratePhysicalDevices(*vulkan_instance->instance, &physical_device_count, devices);

    // so yeah, this is bad
    // TODO: check for device features and use the most suited one
    vulkan_instance->physical_devices = &devices;
    vulkan_instance->physical_device = 0;

    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(devices[0], &device_properties);

    DEBUG_PRINT("Physical device name is '%s' - API v.%d\n", device_properties.deviceName, device_properties.apiVersion);

    vulkan_initialize_queue_families(vulkan_instance);

    return 1;
}

uint8_t vulkan_initialize_logical_device(VulkanInstance_t *vulkan_instance) {
    // we will need the VkDevice later to destroy it, so keep it on the heap I guess?
    VkDevice *device = malloc(sizeof(VkDevice));

    float queue_priority = 1.0f; // TODO: store in VulkanInstance_t (some form of QueueInfo_t array)

    VkDeviceQueueCreateInfo qc_info = {};
    qc_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    qc_info.queueFamilyIndex = vulkan_instance->graphics_queue_family;
    qc_info.queueCount = 1; // TODO: store in VulkanInstance_t
    qc_info.pQueuePriorities = &queue_priority;

    VkPhysicalDeviceFeatures pd_features = {}; // no features for now

    VkDeviceCreateInfo c_info = {};
    c_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    c_info.pQueueCreateInfos = &qc_info;
    c_info.queueCreateInfoCount = 1;
    c_info.pEnabledFeatures = &pd_features;
    c_info.enabledExtensionCount = 0;
#ifdef DEBUG
    //c_info.enabledLayerCount = 0;
    c_info.enabledLayerCount = vulkan_instance->validation_layer_count;
    c_info.ppEnabledLayerNames = vulkan_instance->validation_layers;
#else
    c_info.enabledLayerCount = 0;
#endif

    VkPhysicalDevice physical_device = *vulkan_instance->physical_devices[vulkan_instance->physical_device];
    if (vkCreateDevice(physical_device, &c_info, NULL, device) != VK_SUCCESS) {
        DEBUG_PRINT("Failed to create logical device");
        return 0;
    }

    printf("ADDRESS OF LOGICAL DEVICE %p", device);
    vulkan_instance->logical_device = device;

    return 1;
}

uint8_t vulkan_initialize(GameWindow_t *window, VulkanInstance_t *vulkan_instance) {
    uint32_t glfw_extension_count = 0;
    const char **glfw_extensions;
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    // check for all available extensions
    uint32_t extension_count = query_extension_count();
    VkExtensionProperties extensions[extension_count];
    enumerate_extensions(extension_count, extensions);

    // check for all available validation layers
    uint32_t validation_layer_count = query_validation_layer_count();
    VkLayerProperties layers[validation_layer_count];
    enumerate_validation_layers(validation_layer_count, layers);

    // make sure we support all required glfw extensions
    if (!check_glfw_extensions(&glfw_extension_count, glfw_extensions, extension_count, extensions)) {
        DEBUG_PRINT("Failed to query all required extensions for GLFW\n");
        return 0;
    }

#ifdef DEBUG
    if (!check_validation_layers(validation_layer_count, layers)) {
        DEBUG_PRINT("Failed to query all required validation layers for requested configuration\n");
        return 0;
    }

    vulkan_instance->validation_layer_count = requested_layer_count;
    vulkan_instance->validation_layers = requested_layers;
#endif

    // create application info containing version and names
    VkApplicationInfo a_info;
    a_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    a_info.pApplicationName = "Vulkan Babee";
    a_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    a_info.pEngineName = "Vulkan Engine Babee";
    a_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    a_info.apiVersion = VK_API_VERSION_1_0;

    // create instance creation info containing the application info and the extensions
    VkInstanceCreateInfo c_info;
    c_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    c_info.pApplicationInfo = &a_info;
    c_info.enabledExtensionCount = glfw_extension_count;
    c_info.ppEnabledExtensionNames = glfw_extensions;
    c_info.ppEnabledLayerNames = requested_layers;
    c_info.enabledLayerCount = requested_layer_count;

    // TODO
/*    VkDebugUtilsMessengerCreateInfoEXT debug_create_info;
#ifdef DEBUG
    debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_create_info.

#endif*/

    // create the actual instance using the instance creation info, check for creation failure
    VkInstance *instance = malloc(sizeof(VkInstance));
    if (vkCreateInstance(&c_info, NULL, instance) != VK_SUCCESS) {
        DEBUG_PRINT("failed to create vulkan instance\n");
         return 0;
    }

    // store a reference to the instance in the vulkan-stuff struct
    vulkan_instance->instance = instance;

    // print :)
    DEBUG_PRINT("created instance '%p'\n", instance);

    return 1;
}


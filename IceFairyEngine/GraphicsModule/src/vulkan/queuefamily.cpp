#include "queuefamily.h"

IceFairy::QueueFamily::QueueFamily(vk::PhysicalDevice physicalDevice, VkSurfaceKHR surface) :
	physicalDevice(physicalDevice),
	surface(surface)
{ }

IceFairy::QueueFamily::Indices IceFairy::QueueFamily::FindQueueFamilies(void) {
	Indices indices;

	std::vector<vk::QueueFamilyProperties> queueFamilies = physicalDevice.getQueueFamilyProperties();

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
			indices.graphicsFamily = i;
		}

		/* TODO:
		Note that it's very likely that these end up being the same queue family after all,
		but throughout the program we will treat them as if they were separate queues for a
		uniform approach. Nevertheless, you could add logic to explicitly prefer a physical
		device that supports drawing and presentation in the same queue for improved
		performance.*/
		// Note: Above may be fine due to concurrency
		vk::Bool32 presentSupport = false;
		physicalDevice.getSurfaceSupportKHR(i, surface, &presentSupport);

		if (queueFamily.queueCount > 0 && presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}

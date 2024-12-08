#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <optional>
#include <set>
#include <algorithm>
#include <array>

//现在我们的渲染循环有一个明显的缺陷。我们必须等待前一帧完成后才能开始渲染下一帧，这导致主机不必要的闲置。

namespace eb
{
	class EbSetup;

	class EbSyncObject
	{
	private:
		EbSetup* SetupManager;

	public:
		EbSyncObject(
			VkDevice device,
			EbSetup* manager
		);
		~EbSyncObject();

	private:
		VkDevice device;
		VkSemaphore imageAvailableSemaphore;
		VkSemaphore renderFinishedSemaphore;
		VkFence inFlightFence;

		void createSyncObjects(VkDevice device);

	};
}
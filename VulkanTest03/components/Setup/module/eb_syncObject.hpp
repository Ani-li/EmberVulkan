#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <optional>
#include <set>
#include <algorithm>
#include <array>

//�������ǵ���Ⱦѭ����һ�����Ե�ȱ�ݡ����Ǳ���ȴ�ǰһ֡��ɺ���ܿ�ʼ��Ⱦ��һ֡���⵼����������Ҫ�����á�

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
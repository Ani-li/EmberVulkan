#include "../eb_setup.hpp"
#include "eb_syncObject.hpp"


namespace eb
{
    EbSyncObject::EbSyncObject(
        VkDevice device,
        EbSetup* manager
    ) :SetupManager(manager), device(device)
    {
        createSyncObjects(device);
    }

    EbSyncObject::~EbSyncObject()
    {
        vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
        vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
        vkDestroyFence(device, inFlightFence, nullptr);
    }

    void EbSyncObject::createSyncObjects(VkDevice device)
    {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }

        if (SetupManager)
        {
            SetupManager->Init_imageAvailableSemaphore = imageAvailableSemaphore;
            SetupManager->Init_renderFinishedSemaphore = renderFinishedSemaphore;
            SetupManager->Init_inFlightFence = inFlightFence;
        }

    }
}
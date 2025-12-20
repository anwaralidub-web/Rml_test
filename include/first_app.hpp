#pragma once

#include "lve_window.hpp"
#include "lve_device.hpp"
#include "lve_swap_chain.hpp"
#include  <iostream>
#include  "TextureManager.hpp"
#include <memory>
#include <vector>

namespace lve {

    class FirstApp {
    public:
        FirstApp();
        ~FirstApp();

        FirstApp(const FirstApp&) = delete;
        FirstApp& operator=(const FirstApp&) = delete;

        void run();

    private:
        void loadModels();
        void createDescriptorPool();
        void createDescriptorSet();
        void updateDescriptorSet();
        void createDescriptorSetLayout();
        void createPipelineLayout();
        void createPipeline();
        void recreateSwapChain();
        void createCommandBuffers();
        void freeCommandBuffers();
        void recordCommandBuffer(int imageIndex);
        void drawFrame();
        VkDescriptorSetLayout descriptorSetLayout{};
        VkDescriptorPool descriptorPool;
        VkDescriptorSet descriptorSet;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
    };
}

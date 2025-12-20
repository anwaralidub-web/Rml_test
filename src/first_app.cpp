#include "first_app.hpp"

#include "lve_model.hpp"
#include "lve_pipeline.hpp"

namespace lve {

    FirstApp::FirstApp() {
        LveWindow::init(800, 600, "First App");
        LveDevice::init(LveWindow::get());
        TextureManager::init(LveDevice::get());
        auto& tex = TextureManager::get().loadTexture("resources/textures/main_menu/800x600.png");
        std::cout << "Texture loaded: " << tex.getImageView() << ", " << tex.getSampler() << std::endl;
        loadModels();
        createDescriptorSetLayout();
        createDescriptorPool();
        createDescriptorSet();
        updateDescriptorSet();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }

    FirstApp::~FirstApp() {
        std::cout << "[FirstApp] Mega Destructor Started\n";

        // Call cleanup directly on singletons
        TextureManager::get().cleanup();
        LveModel::get().cleanup();
        LvePipeline::get().cleanup();
        LveSwapChain::get().cleanup();

        std::cout << "[FirstApp] Destroying descriptor stuff...\n";
        vkDestroyDescriptorPool(LveDevice::get().device(), descriptorPool, nullptr);
        vkDestroyDescriptorSetLayout(LveDevice::get().device(), descriptorSetLayout, nullptr);
        vkDestroyPipelineLayout(LveDevice::get().device(), pipelineLayout, nullptr);

        std::cout << "[FirstApp] Destroying LveDevice...\n";
        LveDevice::get().cleanup();

        std::cout << "[FirstApp] Mega Destructor Finished\n";
    }





    void FirstApp::run() {
        while (!LveWindow::get().shouldClose()) {
            glfwPollEvents();
            drawFrame();
        }
        vkDeviceWaitIdle(LveDevice::get().device());
    }

    void FirstApp::loadModels() {
        std::vector<LveModel::Vertex> vertices = {
            // First triangle
            {{-1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // bottom-left
            {{1.0f, -1.0f},  {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}, // bottom-right
            {{1.0f, 1.0f},   {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}, // top-right

            // Second triangle
            {{-1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // bottom-left
            {{1.0f, 1.0f},   {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}, // top-right
            {{-1.0f, 1.0f},  {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}, // top-left
        };

        LveModel::init(LveDevice::get(), vertices);
        std::cout << "Vertex count: " << LveModel::get().getVertexCount() << std::endl;
    }

    void FirstApp::createDescriptorPool() {
        VkDescriptorPoolSize poolSize{};
        poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSize.descriptorCount = 1; // number of descriptors needed

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = 1;

        if (vkCreateDescriptorPool(LveDevice::get().device(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }

    void FirstApp::createDescriptorSet() {
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &descriptorSetLayout;

        if (vkAllocateDescriptorSets(LveDevice::get().device(), &allocInfo, &descriptorSet) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor set!");
        }
    }

    void FirstApp::updateDescriptorSet() {
        auto& tex = TextureManager::get().loadTexture("resources/textures/main_menu/800x600.png");
        std::cout << "Texture loaded From Cache: " << tex.getImageView() << ", " << tex.getSampler() << std::endl;
        VkImageView imageView = tex.getImageView();
        VkSampler sampler = tex.getSampler();
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = imageView;
        imageInfo.sampler = sampler;

        std::cout << "DescriptorSet ImageView: " << imageInfo.imageView
          << ", Sampler: " << imageInfo.sampler << std::endl;

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSet;
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(LveDevice::get().device(), 1, &descriptorWrite, 0, nullptr);
    }

    void FirstApp::createDescriptorSetLayout() {
        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 0;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &samplerLayoutBinding;

        if (vkCreateDescriptorSetLayout(LveDevice::get().device(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
    }

    void FirstApp::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(LveDevice::get().device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void FirstApp::createPipeline() {
        PipelineConfigInfo pipelineConfig{};
        LvePipeline::dafaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = LveSwapChain::get().getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        LvePipeline::init(LveDevice::get(), "resources/shaders/simple_shader.vert.spv",
            "resources/shaders/simple_shader.frag.spv", pipelineConfig);
    }

    void FirstApp::recreateSwapChain() {
        auto extent = LveWindow::get().getExtent();

        // Handle minimized window
        while (extent.width == 0 || extent.height == 0) {
            extent = LveWindow::get().getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(LveDevice::get().device());

        // Recreate the global swapchain
        LveSwapChain::init(LveDevice::get(), extent);
        std::cout << "SwapChain image count: " << LveSwapChain::get().imageCount() << std::endl;

        // Sync command buffers if image count changed
        if (LveSwapChain::get().imageCount() != commandBuffers.size()) {
            freeCommandBuffers();
            createCommandBuffers();
        }

        // Recreate pipeline since swapchain/framebuffer changed
        createPipeline();
    }

    void FirstApp::createCommandBuffers() {
        commandBuffers.resize(LveSwapChain::get().imageCount());
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = LveDevice::get().getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        std::cout << "Command buffers size: " << commandBuffers.size() << std::endl;

        if (vkAllocateCommandBuffers(LveDevice::get().device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void FirstApp::freeCommandBuffers() {
        if (!commandBuffers.empty()) {
            vkFreeCommandBuffers(LveDevice::get().device(),
                                 LveDevice::get().getCommandPool(),
                                 static_cast<uint32_t>(commandBuffers.size()),
                                 commandBuffers.data());
            commandBuffers.clear();
        }
    }

    void FirstApp::recordCommandBuffer(int imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = LveSwapChain::get().getRenderPass();
        renderPassInfo.framebuffer = LveSwapChain::get().getFrameBuffer(imageIndex);
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = LveSwapChain::get().getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float> (LveSwapChain::get().getSwapChainExtent().width);
        viewport.height = static_cast<float> (LveSwapChain::get().getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, LveSwapChain::get().getSwapChainExtent()};
        vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

        LvePipeline::get().bind(commandBuffers[imageIndex]);
        LveModel::get().bind(commandBuffers[imageIndex]);
        vkCmdBindDescriptorSets(
                commandBuffers[imageIndex],
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                pipelineLayout,
                0, 1, &descriptorSet,
                0, nullptr
        );
        LveModel::get().draw(commandBuffers[imageIndex]);

        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    void FirstApp::drawFrame() {
        uint32_t imageIndex;
        auto result = LveSwapChain::get().acquireNextImage(&imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }
        recordCommandBuffer(imageIndex);
        result = LveSwapChain::get().submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || LveWindow::get().wasWindowResized()) {
            LveWindow::get().resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to submit command buffers!");
        }
    }
}

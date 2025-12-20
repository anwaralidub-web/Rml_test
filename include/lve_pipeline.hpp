#pragma once

#include "lve_device.hpp"
#include <string>
#include <vector>

namespace lve {

    struct PipelineConfigInfo {
        PipelineConfigInfo() = default;

        PipelineConfigInfo(const PipelineConfigInfo &) = default;

        PipelineConfigInfo &operator=(const PipelineConfigInfo &) = delete;

        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState blendAttachmentState;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStatesEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
    };

    class LvePipeline {
    public:

        static void init(LveDevice &device, const std::string &vertFilePath, const std::string &fragFilePath, const PipelineConfigInfo configInfo);
        static LvePipeline& get();
        LvePipeline(
                LveDevice &device,
                const std::string &vertFilePath,
                const std::string &fragFilePath,
                const PipelineConfigInfo configInfo);

        ~LvePipeline()  = default;
        void cleanup();

        LvePipeline(const LvePipeline &) = delete;

        LvePipeline &operator=(const LvePipeline &) = delete;

        void bind(VkCommandBuffer commandBuffer);

        static void dafaultPipelineConfigInfo(PipelineConfigInfo &configInfo);

    private:
        static std::vector<char> readFile(const std::string &filepath);

        void createGraphicsPipeline(const std::string &vertFilePath, const std::string &fragFilePath,
                                    const PipelineConfigInfo &configInfo);


        void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

        static std::unique_ptr<LvePipeline> instance;  // declaration only
        LveDevice &lveDevice;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;

    };
}
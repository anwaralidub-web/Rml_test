#pragma once

#include "lve_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "glm/glm.hpp"
#include <vector>

namespace lve {
    class LveModel {
    public:
        struct Vertex {
            glm::vec2 position;
            glm::vec3 color;
            glm::vec2 uv;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();

            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };
        static void init(LveDevice &device, const std::vector<Vertex> &vertices);
        static LveModel& get();

        LveModel(LveDevice &device, const std::vector<Vertex> &vertices);

        ~LveModel() = default;
        void cleanup();

        LveModel(const LveModel &) = delete;

        LveModel &operator=(const LveModel &) = delete;

        void bind(VkCommandBuffer commandBuffer);

        void draw(VkCommandBuffer commandBuffer);
        uint32_t getVertexCount() const { return vertexCount; }

    private:
        void createVertexBuffer(const std::vector<Vertex> &vertices);

        static std::unique_ptr<LveModel> instance;  // declaration only
        LveDevice &lveDevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
    };
}
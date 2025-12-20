#pragma once

#include "lve_device.hpp"
#include <string>
#include <vulkan/vulkan.h>

namespace lve {

    class Texture {
    public:
        Texture(LveDevice &device, const std::string &filepath);

        ~Texture()  = default;
        void cleanup();

        VkImageView getImageView() const { return textureImageView; }

        VkSampler getSampler() const { return textureSampler; }

    private:
        void createTextureImage(const std::string &filepath);

        void createTextureImageView();

        void createTextureSampler();

        LveDevice &lveDevice;

        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
        VkSampler textureSampler;
    };

}  // namespace lve

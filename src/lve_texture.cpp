#include "lve_texture.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <stb_image.h>
#include <stdexcept>

namespace lve {

    Texture::Texture(LveDevice &device, const std::string &filepath) : lveDevice(device) {
        createTextureImage(filepath);
        createTextureImageView();
        createTextureSampler();
    }

    // Texture::~Texture() {
    //     std::cout << "[Texture] Destroying sampler: " << textureSampler << std::endl;
    //     vkDestroySampler(lveDevice.device(), textureSampler, nullptr);
    //
    //     std::cout << "[Texture] Destroying image view: " << textureImageView << std::endl;
    //     vkDestroyImageView(lveDevice.device(), textureImageView, nullptr);
    //
    //     std::cout << "[Texture] Destroying image: " << textureImage << std::endl;
    //     vkDestroyImage(lveDevice.device(), textureImage, nullptr);
    //
    //     std::cout << "[Texture] Freeing memory: " << textureImageMemory << std::endl;
    //     vkFreeMemory(lveDevice.device(), textureImageMemory, nullptr);
    // }


    void Texture::cleanup() {
        std::cout << "[Texture] Destroying sampler: " << textureSampler << "\n";
        vkDestroySampler(lveDevice.device(), textureSampler, nullptr);

        std::cout << "[Texture] Destroying image view: " << textureImageView << "\n";
        vkDestroyImageView(lveDevice.device(), textureImageView, nullptr);

        std::cout << "[Texture] Destroying image: " << textureImage << "\n";
        vkDestroyImage(lveDevice.device(), textureImage, nullptr);

        std::cout << "[Texture] Freeing memory: " << textureImageMemory << "\n";
        vkFreeMemory(lveDevice.device(), textureImageMemory, nullptr);
    }


    void Texture::createTextureImage(const std::string &filepath) {
        int texWidth, texHeight, texChannels;
        // stbi_set_flip_vertically_on_load(true);
        stbi_uc *pixels = stbi_load(filepath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        if (!pixels) {
            throw std::runtime_error("failed to load texture image!");
        }

        VkDeviceSize imageSize = texWidth * texHeight * 4;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        // Create staging buffer with CPU visible memory
        lveDevice.createBuffer(
                imageSize,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                stagingBuffer,
                stagingBufferMemory);

        // Copy pixel data to staging buffer memory
        void *data;
        vkMapMemory(lveDevice.device(), stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
        vkUnmapMemory(lveDevice.device(), stagingBufferMemory);

        stbi_image_free(pixels);

        // Create GPU local image with transfer destination & sampled usage
        lveDevice.createImage(
                texWidth,
                texHeight,
                VK_FORMAT_R8G8B8A8_SRGB,
                VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                textureImage,
                textureImageMemory);

        // Transition image layout to receive data
        lveDevice.transitionImageLayout(textureImage,
                                        VK_FORMAT_R8G8B8A8_SRGB,
                                        VK_IMAGE_LAYOUT_UNDEFINED,
                                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        // Copy buffer (CPU side) to image (GPU side)
        lveDevice.copyBufferToImage(stagingBuffer, textureImage,
                                    static_cast<uint32_t>(texWidth),
                                    static_cast<uint32_t>(texHeight), 1);

        // Transition image layout for shader read access
        lveDevice.transitionImageLayout(textureImage,
                                        VK_FORMAT_R8G8B8A8_SRGB,
                                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        vkDestroyBuffer(lveDevice.device(), stagingBuffer, nullptr);
        vkFreeMemory(lveDevice.device(), stagingBufferMemory, nullptr);
    }

    void Texture::createTextureImageView() {
        textureImageView = lveDevice.createImageView(textureImage,
                                                     VK_FORMAT_R8G8B8A8_SRGB,
                                                     VK_IMAGE_ASPECT_COLOR_BIT);
    }

    void Texture::createTextureSampler() {
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = 16.0f;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

        if (vkCreateSampler(lveDevice.device(), &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }
    }

}  // namespace lve

#pragma once

#include <iostream>

#include "lve_texture.hpp"
#include "lve_device.hpp"
#include <unordered_map>
#include <memory>
#include <string>
#include <bits/ostream.tcc>

namespace lve {

    class TextureManager {
    public:
        // Initialize singleton (call once at startup)
        static void init(LveDevice& device) {
            if (!instance_) {
                instance_ = std::make_unique<TextureManager>(device);
                std::cout << "Created TextureManager instance" << std::endl;
            }
        }

        // Access singleton
        static TextureManager& get() {
            if (!instance_) throw std::runtime_error("TextureManager not initialized!");
            return *instance_;
        }

        TextureManager(LveDevice& device) : lveDevice(device) {}
        ~TextureManager()  = default;
        // Load texture by filepath (returns reference)
        Texture& loadTexture(const std::string& filepath);

        // Optional cleanup
        void cleanup();
    private:

        LveDevice& lveDevice;
        std::unordered_map<std::string, std::unique_ptr<Texture>> textures;

        static std::unique_ptr<TextureManager> instance_;
    };

} // namespace lve

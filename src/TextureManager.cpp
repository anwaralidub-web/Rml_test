#include "TextureManager.hpp"

namespace lve {

    // TextureManager::~TextureManager() {
    //     std::cout << "[TextureManager] Cleaning up textures..." << std::endl;
    //     for (auto &texPair : textures) {
    //         std::cout << "  Texture: " << texPair.first << std::endl;
    //     }
    //     cleanup();
    //     std::cout << "[TextureManager] Cleanup complete." << std::endl;
    // }

    // Initialize static singleton pointer
    std::unique_ptr<TextureManager> TextureManager::instance_ = nullptr;

    // Load texture or return cached one
    Texture& TextureManager::loadTexture(const std::string& filepath) {
        auto it = textures.find(filepath);
        if (it != textures.end()) {
            // Already loaded → reuse
            return *it->second;
        }

        // Not loaded → create new texture
        auto tex = std::make_unique<Texture>(lveDevice, filepath);
        Texture* texPtr = tex.get();
        textures[filepath] = std::move(tex);

        return *texPtr;
    }

    void TextureManager::cleanup() {
        std::cout << "[TextureManager] Cleaning up textures...\n";
        for (auto& pair : textures) {
            auto& tex = pair.second;
            tex->cleanup();  // Each Texture knows how to destroy itself
        }
        textures.clear();
        std::cout << "[TextureManager] Cleanup complete\n";
    }


} // namespace lve

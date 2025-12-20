#pragma once

#define GLFW_INCLUDE_VULKAN
#include <memory>

#include "GLFW/glfw3.h"

#include <string>

namespace lve {

    class LveWindow {
    public:
        static void init(int w, int h, const std::string& titleName);
        static LveWindow& get();
        LveWindow(int w, int h, const std::string& name);

        ~LveWindow();

        bool shouldClose() { return glfwWindowShouldClose(window); }

        VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

        bool wasWindowResized() { return framebufferResized; }

        void resetWindowResizedFlag() { framebufferResized = false; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
    private:
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

        void initWindow();

        int width;
        int height;
        bool framebufferResized = false;
        GLFWwindow *window;
        static std::unique_ptr<LveWindow> instance;  // declaration only

        std::string title_;
    };
}  // namespace lve
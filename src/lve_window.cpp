#include "lve_window.hpp"

// std
#include <iostream>
#include <stdexcept>

namespace lve {

    std::unique_ptr<LveWindow> LveWindow::instance = nullptr;

    void LveWindow::init(int w, int h, const std::string& titleName) {
        if (!instance) {
            instance = std::make_unique<LveWindow>(w, h, titleName);
            std::cout << "Window Initalized: " << titleName << std::endl;
        }
    }

    LveWindow& LveWindow::get() {
        if (!instance) throw std::runtime_error("Window not initialized!");
        return *instance;
    }

    LveWindow::LveWindow(int w, int h, const std::string& name) : width{w}, height{h}, title_{name} {
        initWindow();
    }

    LveWindow::~LveWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void LveWindow::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, title_.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    }

    void LveWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void LveWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto lveWindow = reinterpret_cast<LveWindow *>(glfwGetWindowUserPointer(window));
        lveWindow->framebufferResized = true;
        lveWindow->width = width;
        lveWindow->height = height;
    }

}  // namespace lve
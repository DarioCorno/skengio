#pragma once

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <string>

namespace SKEngio {

    struct WindowSpecs {
        unsigned int width;
        unsigned int height;
        std::string caption;
        bool fullscreen;
    };

    class WindowManager {
        public:

        GLFWwindow* window;

        unsigned int width;
        unsigned int height;
        std::string caption;

        WindowManager(WindowSpecs* windowSpecs);

        // prevent copying object
        WindowManager(const WindowManager&) = delete;
        WindowManager(WindowManager&&) = delete;
        WindowManager& operator=(const WindowManager&) = delete;
        WindowManager& operator=(WindowManager&&) = delete;

        ~WindowManager();

    };
}
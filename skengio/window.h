#pragma once

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <string>

#include "skengio/utils/singleton.h"

namespace SKEngio {

    struct WindowSpecs {
        unsigned int width;
        unsigned int height;
        std::string caption;
        bool canResize;
        bool fullscreen;
    };

    class WindowManager final : public Singleton<WindowManager> {
        public:

        GLFWwindow* window;

        unsigned int width;
        unsigned int height;
        std::string caption;

        //WindowManager(WindowSpecs* windowSpecs);
        
        void Init(WindowSpecs* windowSpecs);
        void OnDestroy();

        // prevent copying object
        //WindowManager(const WindowManager&) = delete;
        //WindowManager(WindowManager&&) = delete;
        //WindowManager& operator=(const WindowManager&) = delete;
        //WindowManager& operator=(WindowManager&&) = delete;

        //~WindowManager();

    };
}
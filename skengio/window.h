#pragma once

#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

using namespace std;


namespace SKEngio {

    struct WindowSpecs {
        unsigned int width;
        unsigned int height;
        string caption;
        bool fullscreen;
    };

    class WindowManager {
        public:

        GLFWwindow* window;

        unsigned int width;
        unsigned int height;
        string caption;

        WindowManager(WindowSpecs* windowSpecs);

        virtual ~WindowManager();

    };
}
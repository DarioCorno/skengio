#include "window.h"

#include <stdlib.h>
#include <string>
#include <iostream>

#include "logger.h"

namespace SKEngio {

    WindowManager::WindowManager(WindowSpecs* windowSpecs) {

        if (!glfwInit()) {
            SK_LOG("ERROR GLFW Cannot initialize GLFW.");
            exit(EXIT_FAILURE);
        }

        caption = windowSpecs->caption;
        
        if (windowSpecs->fullscreen) {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();

            //get  current screen mode
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            width = mode->width;
            height = mode->height;

            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

            window = glfwCreateWindow(width, height, this->caption.c_str(), monitor, NULL);
        }
        else {
            width = windowSpecs->width;
            height = windowSpecs->height;
            window = glfwCreateWindow(this->width, this->height, this->caption.c_str(), NULL, NULL);
        }

        if(window == NULL) {
            std::cout << "GLFW ERROR window init failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        SK_LOG("GLFW Initializing window (" << width << "x" << height << ")");

    }

    WindowManager::~WindowManager() {
        std::cout << "Destroying window" << std::endl;
		glfwDestroyWindow(this->window);
		glfwTerminate();
    }

}
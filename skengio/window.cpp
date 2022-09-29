#include "core.h"
#include "window.h"

#include <stdlib.h>
#include <string>
#include <iostream>

using namespace std;

namespace SKEngio {

    WindowManager::WindowManager(WindowSpecs* windowSpecs) {
        this->width = windowSpecs->width;
        this->height = windowSpecs->height;
        this->caption = windowSpecs->caption;
        this->window = glfwCreateWindow(this->width, this->height, this->caption.c_str(), NULL, NULL);
        if(window == NULL) {
            std::cout << "GLFW ERROR window init failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        SK_LOG("GLFW Initializing window (" << this->width << "x" << this->height << ")");

    }

    WindowManager::~WindowManager() {
        std::cout << "Destroying window" << std::endl;
		glfwDestroyWindow(this->window);
		glfwTerminate();
    }

}
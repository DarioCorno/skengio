#pragma once

#include <iostream>
#include <string>

using namespace std;

#include "core.h"

//#include "window.h"
//#include "application.h"
//#include "renderer.h"

namespace SKEngio {

    class Application {
        public:

        Application(WindowSpecs* windowSpecs, void (* errorCB)(int, const char*) );

        virtual ~Application();

        void Run();

        void HandleKeyboardEvent(int key, int scancode, int action, int mods);
        void HandleResizeEvent(int iWidth, int iHeight);
        void HandleMouseButtonEvent(int button, int action, int mods);
        void HandleMouseMoveEvent(double xPos, double yPos);
        void HandleMouseScrollEvent(double xOffset, double yOffset);

        GLFWwindow* GetWindow();
        Renderer* GetRenderer();

        private:

        WindowManager* winMan;
        Renderer* renderer;
    };
}
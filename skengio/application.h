#pragma once

#ifndef _SK_APPLICATION_
#define _SK_APPLICATION_

#include <iostream>
#include <string>

using namespace std;

#include "core.h"

namespace SKEngio {

    class Renderer;
    
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
            SKEngio::Renderer* GetRenderer();

        private:

            WindowManager* winMan;
            SKEngio::Renderer* renderer;
    };
}

#endif
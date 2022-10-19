#pragma once

#ifndef SK_APPLICATION_
#define SK_APPLICATION_

#include "window.h"

namespace SKEngio {

    class Renderer;
    
    class Application {
        public:

            Application(WindowSpecs* windowSpecs, void (* errorCB)(int, const char*) );

            Application(const Application&) = delete;
            Application(Application&&) = delete;
            Application& operator=(const Application&) = delete;
            Application& operator=(Application&&) = delete;

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
            Renderer* renderer;
    };
}

#endif
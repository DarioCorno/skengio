#pragma once

#ifndef SK_GUIMANAGER_
#define SK_GUIMANAGER_

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "event.h"

namespace SKEngio {
    class Renderer;

    class GUIManager {
        public:
            explicit GUIManager(Renderer* parentR);

            GUIManager(const GUIManager&) = delete;
            GUIManager(GUIManager&&) = delete;
            GUIManager& operator=(const GUIManager&) = delete;
            GUIManager& operator=(GUIManager&&) = delete;

            ~GUIManager();

            void InitGUI(GLFWwindow* window );

            void DrawBegin();
            void Draw();
            void DrawEnd(GLFWwindow* window);
            void DrawSwapBuffers();

            void OnEvent(Event* e);

        private:
            Renderer* parentRenderer;
            int winWidth, winHeight;
            const char* glsl_version = "#version 130";  //GLSL used by ImGui
            bool logVisible = true;
    };
}

#endif
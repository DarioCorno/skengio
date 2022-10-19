#pragma once

#ifndef _SK_GUIMANAGER_
#define _SK_GUIMANAGER_

#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "event.h"
#include "defines.h"
#include "logger.h"
#include "scene.h"
#include "renderer.h"
#include "renderParams.h"

namespace SKEngio {

    class Renderer;

    class GUIManager {
        public:

            GUIManager(Renderer* parentR);

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
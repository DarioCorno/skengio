#pragma once

#include <GLFW/glfw3.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "event.h"
#include "defines.h"
#include "logger.h"

namespace SKEngio {

    class GUIManager {
        public:

        GUIManager();

        ~GUIManager();

        void InitGUI(GLFWwindow* window );

        void Destroy();

        void DrawBegin();
        void Draw();
        void DrawEnd(GLFWwindow* window);
        void DrawSwapBuffers();

        void OnEvent(Event* e);

        private:
        int winWidth, winHeight;
        const char* glsl_version = "#version 130";
    };
}
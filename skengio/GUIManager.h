#pragma once

#include <GLFW/glfw3.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "event.h"
#include "defines.h"
#include "logger.h"
#include "scene.h"
#include "renderer.h"

namespace SKEngio {

    class Renderer;

    class GUIManager {
        public:

        GUIManager(Renderer* parentR);

        ~GUIManager();

        void InitGUI(GLFWwindow* window );

        void Destroy();

        void DrawBegin();
        void Draw();
        void DrawEnd(GLFWwindow* window);
        void DrawSwapBuffers();

        void OnEvent(Event* e);

        void ToggleLog();

        private:
        Renderer* parentRenderer;
        int winWidth, winHeight;
        const char* glsl_version = "#version 130";
        bool logVisible = true;
    };
}
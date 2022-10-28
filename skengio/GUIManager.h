#pragma once

#ifndef SK_GUIMANAGER_
#define SK_GUIMANAGER_

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "event.h"
#include "skengio/utils/singleton.h"
#include "skengio/layer.h"

namespace SKEngio {
    class Renderer;

    class GUIManager final : public Singleton<GUIManager>{
        public:

            void InitGUI();
            void Destroy();

            void DrawBegin();
            void Draw();
            void DrawEnd();
            void DrawSwapBuffers();

            void OnEvent(Event* e);

        private:
            int winWidth, winHeight;
            const char* glsl_version = "#version 130";  //GLSL used by ImGui
            bool logVisible = true;
    };
}

#endif
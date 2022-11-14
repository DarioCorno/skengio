#pragma once

#ifndef SK_GUIMANAGER_
#define SK_GUIMANAGER_

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

//https://github.com/AirGuanZ/imgui-filebrowser
#include "skengio/gui/imfilebrowser.h"

#include "event.h"
#include "skengio/utils/singleton.h"
#include "skengio/layer.h"
#include "material.h"

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

            void DrawMaterialParams(const std::string matName, Material* material);

        private:
            int winWidth, winHeight;
            const char* glsl_version = "#version 130";  //GLSL used by ImGui
            bool logVisible = false;

            Material* curEditMaterial = nullptr;

            ImGui::FileBrowser fileDialog;

    };
}

#endif
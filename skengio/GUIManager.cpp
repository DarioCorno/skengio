#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "GUIManager.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>


namespace SKEngio {

    class RenderParams;

    GUIManager::GUIManager(Renderer* parentR) {
        parentRenderer = parentR;
    }

    GUIManager::~GUIManager() {        
        std::cout << "Destroying GUImanager" << std::endl;

        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

    }

    void GUIManager::InitGUI(GLFWwindow* _window ) {
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(_window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        glfwGetWindowSize(_window, &winWidth, &winHeight);

    }

    void GUIManager::DrawBegin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void GUIManager::Draw() {
            static int counter = 0;

            if(GUI_SHOW_FPS) {
                ImGui::SetNextWindowPos(ImVec2(2, 2), ImGuiCond_FirstUseEver );
                ImGui::SetNextWindowBgAlpha(0.2f);
                ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
                ImGui::Begin("SKEngio", NULL, window_flags);

                ImGui::Text("App average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();                    
            }

            if(logVisible) {
                Logger* log = SKEngio::Logger::getInstance();
                ImGui::SetNextWindowPos(ImVec2(0, winHeight - 150), 1 );
                ImGui::SetNextWindowSize(ImVec2(winWidth, 150), 1 );
                ImGui::SetNextWindowBgAlpha(0.2f);
                ImGuiWindowFlags log_window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
                ImGui::Begin("Application Log", NULL, log_window_flags);

                for(LogEntry le : log->buffer) {
                    if(le.type == LOG_INFO) {
                        ImGui::Text( "%s" , le.entry.c_str() );
                    } else {
                        ImGui::TextColored( ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s" , le.entry.c_str() );
                    }
                }
                
                if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0f);

                ImGui::End();
            }

            //scenes rows
            ImGui::SetNextWindowPos(ImVec2(winWidth - 200, 0), 1 );
            ImGui::SetNextWindowSize(ImVec2(200, winHeight - ((logVisible) ? 150 : 0)), 1 );
            ImGui::SetNextWindowBgAlpha(0.2f);
            ImGuiWindowFlags log_window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
            ImGui::Begin("Settings", NULL, log_window_flags);
            ImGui::Text("Audio");
            ImGui::Separator();
            //music is in scene 0 (horrible)
            Scene* scene0 = parentRenderer->GetSceneStack()->scenes[0];
            ImGui::PlotLines("dB", scene0->music->getFFT(), 512);
            ImGui::Text("Scenes");
            ImGui::Separator();
            for(Scene* scn : parentRenderer->GetSceneStack()->scenes) {
                if ( ImGui::TreeNode( scn->dispName.c_str()) )
                {
                    for(Layer* lyr : scn->GetLayerStack()->layers ) {
                        std::string lyrName = "Layer " + std::to_string( lyr->GetId() );
                        ImGui::Checkbox( lyrName.c_str() , &lyr->enabled );
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::Separator();
            ImGui::Checkbox("Use FBO", &parentRenderer->renderParams->useVBO);
            ImGui::Separator();
            ImGui::Checkbox("Shadows", &parentRenderer->renderParams->useShadows);
            ImGui::Separator();
            ImGui::Checkbox( "Show Log", &logVisible );
            
            ImGui::End();


    }

    void GUIManager::DrawEnd(GLFWwindow* window) {
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        //glViewport(0, 0, display_w, display_h);

        //glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void GUIManager::DrawSwapBuffers() {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void GUIManager::OnEvent(Event* e) {
        ImGuiIO& io = ImGui::GetIO();
        switch(e->type) {
            case EVENT_TYPE_KEYPRESS:
                break;
            case EVENT_TYPE_KEYRELEASE:
                break;
            case EVENT_TYPE_MOUSEPRESS:
                io.MouseDown[ e->button ] = true;
                break;
            case EVENT_TYPE_MOUSERELEASE:
                io.MouseDown[ e->button ] = false;
                break;
            case EVENT_TYPE_MOUSEMOVE:
                io.MousePos = ImVec2( e->xPos, e->yPos );
                break;
            case EVENT_TYPE_MOUSESCROLL:
                break;
            case EVENT_TYPE_RESIZE:
                winWidth = e->width;
                winHeight = e->height;
                break;
        }
    }

}
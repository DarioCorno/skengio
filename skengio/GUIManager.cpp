#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "GUIManager.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>

#include "defines.h"
#include "logger.h"
#include "renderer.h"
#include "scene.h"


#define RIGHT_PANEL_WIDTH 250
#define BOTTOM_PANEL_HEIGHT 150
#define GUI_TEXTURE_SIZE std::round(RIGHT_PANEL_WIDTH / 2.0f)

namespace SKEngio {
    //GUIManager::GUIManager() {
    //    //parentRenderer = parentR;
    //}
    //
    //GUIManager::~GUIManager() {        
    //    std::cout << "Destroying GUImanager" << std::endl;
    //
    //    // Cleanup
    //    ImGui_ImplOpenGL3_Shutdown();
    //    ImGui_ImplGlfw_Shutdown();
    //    ImGui::DestroyContext();
    //
    //}
    
    void GUIManager::InitGUI() {
        
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
        ImGui_ImplGlfw_InitForOpenGL( WindowManager::get().window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        glfwGetWindowSize( WindowManager::get().window, &winWidth, &winHeight);

    }

    void GUIManager::DrawMaterialParams(const std::string matName, Material* material) {
        ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400, 250), 1);
        ImGui::SetNextWindowBgAlpha(0.5f);
        ImGui::Begin(matName.c_str() );
        ImGui::Text("Ambient");
        ImGui::ColorEdit3("ambient", (float*)glm::value_ptr(material->materialAmbientColor));
        ImGui::Text("Diffuse");
        ImGui::ColorEdit3("diffuse", (float*)glm::value_ptr(material->materialDiffuseColor));
        ImGui::Text("Specular:");
        ImGui::ColorEdit3("specular", (float*)glm::value_ptr(material->materialSpecularColor));
        ImGui::Text("Shininess:");
        ImGui::SliderFloat("shininess", &material->materialShininess, 0.0, 128.0);
        ImGui::Text("Reflectivity:");
        ImGui::SliderFloat("reflectivity", &material->materialReflectivity, 0.0f, 1.0f);
        ImGui::End();

    }

    void GUIManager::Destroy() {
        std::cout << "Destroying GUImanager" << std::endl;

        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

    }

    void GUIManager::DrawBegin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void GUIManager::Draw() {

            if(GUI_SHOW_FPS) {
                ImGui::SetNextWindowPos(ImVec2(2, 2), ImGuiCond_FirstUseEver );
                ImGui::SetNextWindowBgAlpha(0.2f);
                ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
                ImGui::Begin("SKEngio", nullptr, window_flags);

                ImGui::Text("App average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();                    
            }

            if(logVisible) {
                Logger& log = SKEngio::Logger::getInstance();
                ImGui::SetNextWindowPos(ImVec2(0, winHeight - BOTTOM_PANEL_HEIGHT), 1 );
                ImGui::SetNextWindowSize(ImVec2(winWidth, BOTTOM_PANEL_HEIGHT), 1 );
                ImGui::SetNextWindowBgAlpha(0.2f);
                ImGuiWindowFlags log_window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
                ImGui::Begin("Application Log", nullptr, log_window_flags);

                for(const LogEntry& le : log.buffer) {
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
            ImGui::SetNextWindowPos(ImVec2(winWidth - RIGHT_PANEL_WIDTH, 0), 1 );
            ImGui::SetNextWindowSize(ImVec2(RIGHT_PANEL_WIDTH, winHeight - ((logVisible) ? BOTTOM_PANEL_HEIGHT : 0)), 1 );
            ImGui::SetNextWindowBgAlpha(0.2f);
            ImGuiWindowFlags log_window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
            ImGui::Begin("Settings", nullptr, log_window_flags);
            ImGui::Text("Audio");
            ImGui::Separator();
            //music is in scene 0 (horrible)
            Scene* scene0 = Renderer::get().GetSceneStack()->scenes[0];
            ImGui::PlotLines("dB", scene0->music->getFFT(), 512);
            ImGui::Text("Scenes");
            //ImGui::Separator();
            for(Scene* scn : Renderer::get().GetSceneStack()->scenes) {
                if ( ImGui::TreeNode( scn->dispName.c_str()) )
                {
                    unsigned int guiIdx = 0;
                    if (ImGui::TreeNode("Lights")) {
                        for (Light* light : scn->lights) {
                            std::string ltName = "Light " + std::to_string(guiIdx);
                            ImGui::Text(ltName.c_str());
                            ImGui::PushID(guiIdx);
                            ImGui::Checkbox("Enabled", &light->enabled);
                            ImGui::Checkbox("Cast Shadows", &light->castShadows);
                            if(light->castShadows && light->enabled)
                                ImGui::Image((void*)(intptr_t)light->GetShadowTexture()->textureID, ImVec2(GUI_TEXTURE_SIZE, GUI_TEXTURE_SIZE));
                            ImGui::PopID();
                            guiIdx++;
                        }
                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNode("Entities")) {
                        for (Entity* ent : scn->entities) {
                            ImGui::Text(ent->id.c_str());
                            ImGui::PushID(guiIdx);
                            //ImGui::Checkbox("Enabled", &light->enabled);
                            //ImGui::Checkbox("Cast Shadows", &light->castShadows);
                            std::string mId = "Material " + std::to_string(ent->material->materialID);
                            if ( ImGui::Button(mId.c_str()) ) {
                                if (curEditMaterial == nullptr) {
                                    curEditMaterial = ent->material;
                                }
                                else {
                                    curEditMaterial = nullptr;
                                }
                            }
                            ImGui::PopID();
                            guiIdx++;
                        }
                        ImGui::TreePop();
                    }

                    ImGui::TreePop();

                }
            }
            ImGui::Separator();
            ImGui::Checkbox("Debug Depth", &Renderer::get().depthDebug);
            ImGui::Separator();
            ImGui::Checkbox("Shadows", &Renderer::get().shadowsDebug);
            ImGui::Separator();
            ImGui::Checkbox( "Show Log", &logVisible );
            
            ImGui::End();

            if (curEditMaterial != nullptr) {
                std::string mId = "Material " + std::to_string(curEditMaterial->materialID);
                DrawMaterialParams(mId, curEditMaterial);
            }
    }

    void GUIManager::DrawEnd() {
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(WindowManager::get().window, &display_w, &display_h);
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
            case EventType::KeyPress:
                break;
            case EventType::KeyRelease:
                break;
            case EventType::MousePress:
                io.MouseDown[ e->button ] = true;
                break;
            case EventType::MouseRelease:
                io.MouseDown[ e->button ] = false;
                break;
            case EventType::MouseMove:
                io.MousePos = ImVec2( e->xPos, e->yPos );
                break;
            case EventType::MouseScroll:
                break;
            case EventType::Resize:
                winWidth = e->width;
                winHeight = e->height;
                break;
        }
    }

}

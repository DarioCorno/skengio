#define GLFW_DLL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "core.h"

#include <vector>
#include <algorithm>

namespace SKEngio {

    Renderer::Renderer(WindowManager* winMan) {
        //keeps a reference to winManager
        this->winMan = winMan;
        

        sceneStack = new SceneStack();

        //initializes the opengl stuff
        this->InitGL();

        //initialize the GUI system
        this->InitGUI();
    }

    Renderer::~Renderer() {
        //TODO: destroy all scenes
        sceneStack->Destroy();

        guiMan->Destroy();
    }

    void Renderer::HandleResize(int width, int height) {
        
        for(Scene* scene : sceneStack->scenes) {
            scene->handleResize(width, height);
        }

    }

    void Renderer::OnEvent(Event* e) {

        //handle event by renderer
        if(e->type == EVENT_TYPE_RESIZE) {
            HandleResize(e->width, e->height);
        }

        //pass the event each scene
        for(Scene* scene : sceneStack->scenes) {
            scene->OnEvent(e);
        }

        //forward to GUI
        guiMan->OnEvent(e);
    }


    void Renderer::InitGUI() {
        guiMan = new GUIManager();
        guiMan->InitGUI(winMan->window);
    }


    bool Renderer::InitGL() {

        glfwMakeContextCurrent(this->winMan->window);

        //set vsync true
        glfwSwapInterval(1);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glewExperimental = GL_TRUE;
		if(glewInit() != GLEW_OK) 
		{
			SK_LOG("GLEW Error - glew Init failed :(");
			return false;
		}

		SK_LOG("OpenGL version: " << glGetString(GL_VERSION) );
        SK_LOG("GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) );
        SK_LOG("Vendor: " << glGetString(GL_VENDOR) );
        SK_LOG("Renderer: " << glGetString(GL_RENDERER) );     

        return true;   
    }

    void Renderer::Draw() {

        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float timeValue = glfwGetTime();

        //begin the gui rendering
        guiMan->DrawBegin();
        guiMan->Draw();
        for(Scene* scene : sceneStack->scenes) {            
            scene->OnDrawGUI(timeValue);
        }
        guiMan->DrawEnd(winMan->window);

        //update and render all scenes
        for(Scene* scene : sceneStack->scenes) {
            scene->UpdateAndDraw(timeValue);
        }

        //end the gui rendering
        guiMan->DrawSwapBuffers();

        glfwMakeContextCurrent(winMan->window);
        glfwSwapBuffers(winMan->window);
    }

    void Renderer::AddScene(Scene* newScene) {
        sceneStack->AddScene(newScene);
    }

}
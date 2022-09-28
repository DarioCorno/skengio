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
        
        //initializes the layers manager
        layerStack = new LayerStack();

        //initializes the opengl stuff
        this->InitGL();

        //initialize the GUI system
        this->InitGUI();
    }

    Renderer::~Renderer() {
        layerStack->Destroy();

        guiMan->Destroy();
    }

    void Renderer::HandleResize(int width, int height) {
        

        activeCamera->handleResize(width, height);

        /* 
		if(height == 0) height = 1;
		float ratio = width / float(height);
		glViewport(0, 0, width, height);

		mProjMatrix = glm::perspective(fovAngle, ratio, nearPlane, farPlane);    
		
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        //setPerspective(fovAngle, ratio, nearPlane, farPlane);
        glMatrixMode(GL_MODELVIEW);            
        */
    }

    void Renderer::OnEvent(Event* e) {

        //handle event by renderer
        if(e->type == EVENT_TYPE_RESIZE) {
            HandleResize(e->width, e->height);
        }

        //pass the event to layerStack
        layerStack->OnEvent(e);

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
			std::cout << "GLEW Error - glew Init failed :(" << std::endl;
			return false;
		}

		std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
        std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;     

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
        for(Layer* layer : layerStack->layers) {
            layer->OnDrawGUI(timeValue);
        }
        guiMan->DrawEnd(winMan->window);

        //update and render all layers
        for(Layer* layer : layerStack->layers) {
            layer->setCamera( activeCamera );
            layer->OnUpdate(timeValue);
            layer->OnDraw(0.0f);
        }

        //end the gui rendering
        guiMan->DrawSwapBuffers();

        glfwMakeContextCurrent(winMan->window);
        glfwSwapBuffers(winMan->window);
    }

    LayerStack* Renderer::GetLayerStack() {
        return layerStack;
    }

    void Renderer::addCamera(Camera* newCamera) {
        cameraList.push_back(newCamera);
    }

    void Renderer::setActiveCamera(unsigned int camID) {
        for(Camera* cam : cameraList) {
            if (cam->id == camID) {
                activeCamera = cam;
                break;
            }
        }   
    }
}
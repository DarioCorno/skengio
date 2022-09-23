#include "window.h"
#include "renderer.h"
#include "application.h"
#include "layer.h"
#include "layerstack.h"
#include "event.h"

static void keyCB (GLFWwindow * window, int key, int scancode, int action, int mods) {
    SKEngio::Application* app = reinterpret_cast<SKEngio::Application *>(glfwGetWindowUserPointer(window));
    if (app)
        app->HandleKeyboardEvent(key, scancode, action, mods);
}

static void resizeCB(GLFWwindow* window, int iWidth, int iHeight)
{
    SKEngio::Application* app = reinterpret_cast<SKEngio::Application *>(glfwGetWindowUserPointer(window));
    if (app)
        app->HandleResizeEvent(iWidth, iHeight);
}

static void mouseButtonCB(GLFWwindow* window, int button, int action, int mods) {
    SKEngio::Application* app = reinterpret_cast<SKEngio::Application *>(glfwGetWindowUserPointer(window));
    if (app)
        app->HandleMouseButtonEvent(button, action, mods);
}

static void mouseMoveCB(GLFWwindow* window, double xPos, double yPos)
{
    SKEngio::Application* app = reinterpret_cast<SKEngio::Application *>(glfwGetWindowUserPointer(window));
    if (app)
        app->HandleMouseMoveEvent(xPos, yPos);
}

static void scrollCB(GLFWwindow* window, double xoffset, double yoffset)
{
    SKEngio::Application* app = reinterpret_cast<SKEngio::Application *>(glfwGetWindowUserPointer(window));
    if (app)
        app->HandleMouseScrollEvent(xoffset, yoffset);
}

namespace SKEngio {

    Application::Application(WindowSpecs* windowSpecs, void (* errorCB)(int, const char*)) {

        glfwSetErrorCallback( errorCB );

        std::cout << "GLFW Initializing" << std::endl;

        if (!glfwInit()) {
            std::cout << "ERROR GLFW Cannot initialize GLFW." << std::endl;
            exit(EXIT_FAILURE);
        }

        //initializes glfw Window
        this->winMan = new SKEngio::WindowManager( windowSpecs );
        
        //initializes OGL stuff for the glfw window
        this->renderer = new SKEngio::Renderer(this->winMan);

        glfwSetWindowUserPointer(winMan->window, reinterpret_cast<void *>(this));
        glfwSetKeyCallback(winMan->window, &keyCB);
        glfwSetWindowSizeCallback(winMan->window, &resizeCB);
        glfwSetCursorPosCallback(winMan->window, &mouseMoveCB);
        glfwSetMouseButtonCallback(winMan->window, &mouseButtonCB);

        //just call the resize once at the beginning
        //renderer->HandleResize(windowSpecs->width, windowSpecs->height);
    }

    Application::~Application() {
        std::cout << "Destroying Application" << std::endl;
        delete renderer;
        delete winMan;
    }

    void Application::HandleKeyboardEvent(int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS)
		{
			switch (key)
			{
				case GLFW_KEY_ESCAPE:
				case GLFW_KEY_Q:
					glfwSetWindowShouldClose(this->winMan->window, GL_TRUE);
					break;
			}

            std::cout << key << " pressed." << std::endl;
            Event* e = new Event();
            e->SetKeyPress(key, scancode, action ,mods);
            renderer->OnEvent(e);
		} else {
            Event* e = new Event();
            e->SetKeyRelease(key, scancode, action ,mods);
            renderer->OnEvent(e);
        }


    }

    void Application::HandleMouseButtonEvent(int button, int action, int mods) {
			switch (action)
			{
				case GLFW_PRESS:
				{
					//std::cout << "Mouse btn " << button << " pressed" << std::endl;
                    Event* e = new Event();
                    e->SetMousePress(button, action ,mods);
                    renderer->OnEvent(e);

					break;
				}
				case GLFW_RELEASE:
				{
					//std::cout << "Mouse btn " << button << " released" << std::endl;
                    Event* e = new Event();
                    e->SetMouseRelease(button, action ,mods);
                    renderer->OnEvent(e);
					break;
				}
            }
    }

    void Application::HandleMouseMoveEvent(double xPos, double yPos) {
        //std::cout << "Mouse:" << xPos << " - " << yPos << std::endl;
        Event* e = new Event();
        e->SetMouseMove(xPos, yPos);
        renderer->OnEvent(e);

    }

    void Application::HandleResizeEvent(int width, int height) {
        Event* e = new Event();
        e->SetResize(width, height);
        renderer->OnEvent(e);
        //renderer->HandleResize(width, height);
        //std::cout << "Resize: " << width << " " << height << std::endl;
    }

    void Application::HandleMouseScrollEvent(double xOffset, double yOffset) {

    }

    void Application::Run() {

        glfwMakeContextCurrent(winMan->window);

        while (!glfwWindowShouldClose(winMan->window))
        {
            this->renderer->Draw();

            glfwPollEvents();	//or glfwWaitEvents()
        }        
    }


    Renderer* Application::GetRenderer() {
        return renderer;
    }

    GLFWwindow* Application::GetWindow() {
        return winMan->window;
    }

}
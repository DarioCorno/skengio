
#include "application.h"

#include "logger.h"
#include "renderer.h"

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

    //Application::Application(WindowSpecs* windowSpecs, void (* errorCB)(int, const char*)) {
    //
    //    glfwSetErrorCallback( errorCB );
    //
    //    //initializes glfw Window
    //    this->winMan = new SKEngio::WindowManager( windowSpecs );
    //    
    //    //initializes OGL stuff 
    //    this->renderer = new SKEngio::Renderer(this->winMan);
    //
    //    glfwSetWindowUserPointer(winMan->window, reinterpret_cast<void *>(this));
    //    glfwSetKeyCallback(winMan->window, &keyCB);
    //    glfwSetWindowSizeCallback(winMan->window, &resizeCB);
    //    glfwSetCursorPosCallback(winMan->window, &mouseMoveCB);
    //    glfwSetMouseButtonCallback(winMan->window, &mouseButtonCB);
    //
    //}

    void Application::Initialize(WindowSpecs* windowSpecs, void (*errorCB)(int, const char*)) {

        glfwSetErrorCallback(errorCB);

        //initializes glfw Window
        //this->winMan = new SKEngio::WindowManager(windowSpecs);
        WindowManager::get().Init(windowSpecs);

        Renderer::get().Init();

        //initializes OGL stuff 
        //this->renderer = new SKEngio::Renderer(this->winMan);
        //Renderer::get().InitWindow(winMan);

        glfwSetWindowUserPointer(WindowManager::get().window, reinterpret_cast<void*>(this));
        glfwSetKeyCallback(WindowManager::get().window, &keyCB);
        glfwSetWindowSizeCallback(WindowManager::get().window, &resizeCB);
        glfwSetCursorPosCallback(WindowManager::get().window, &mouseMoveCB);
        glfwSetMouseButtonCallback(WindowManager::get().window, &mouseButtonCB);

    }

    void Application::Destroy() {
        //delete renderer;
        //delete winMan;
        Renderer::get().OnDestroy();
        WindowManager::get().OnDestroy();
        std::cout << "Destroying Application" << std::endl;
    }

    //Application::~Application() {
    //    delete renderer;
    //    delete winMan;
    //    std::cout << "Destroying Application" << std::endl;
    //}

    void Application::HandleKeyboardEvent(int key, int scancode, int action, int mods) {
        Event* e = new Event();
        if (action == GLFW_PRESS)
		{
			switch (key)
			{
				case GLFW_KEY_ESCAPE:
					glfwSetWindowShouldClose(WindowManager::get().window, GL_TRUE);
					break;
			}

            //SK_LOG(key << " pressed.");
            e->SetKeyPress(key, scancode, action ,mods);

		} else {

            e->SetKeyRelease(key, scancode, action ,mods);
        }

        //renderer->OnEvent(e);
        Renderer::get().OnEvent(e);

    }

    void Application::HandleMouseButtonEvent(int button, int action, int mods) {
            Event* e = new Event();
			switch (action)
			{
				case GLFW_PRESS:
				{
					//std::cout << "Mouse btn " << button << " pressed" << std::endl;                    
                    e->SetMousePress(button, action ,mods);
					break;
				}
				case GLFW_RELEASE:
				{
					//std::cout << "Mouse btn " << button << " released" << std::endl;
                    e->SetMouseRelease(button, action ,mods);
					break;
				}
            }

            //renderer->OnEvent(e);
            Renderer::get().OnEvent(e);
    }

    void Application::HandleMouseMoveEvent(double xPos, double yPos) {
        //std::cout << "Mouse:" << xPos << " - " << yPos << std::endl;
        Event* e = new Event();
        e->SetMouseMove(xPos, yPos);
        //renderer->OnEvent(e);
        Renderer::get().OnEvent(e);

    }

    void Application::HandleResizeEvent(int width, int height) {
        Event* e = new Event();
        e->SetResize(width, height);
        //renderer->OnEvent(e);
        Renderer::get().OnEvent(e);
    }

    void Application::HandleMouseScrollEvent(double xOffset, double yOffset) {

    }

    void Application::Run() {

        SK_LOG("Starting rendering loop.");

        glfwMakeContextCurrent(WindowManager::get().window);

        while (!glfwWindowShouldClose(WindowManager::get().window))
        {

            //this->renderer->Draw();
            Renderer::get().Draw();

            glfwPollEvents();	//or glfwWaitEvents()
        }                
    }


    //Renderer* Application::GetRenderer() {
    //    return renderer;
    //}

    //GLFWwindow* Application::GetWindow() {
    //    return WindowManager::get().window;
    //}

}
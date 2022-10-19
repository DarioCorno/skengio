#include "renderer.h"

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <algorithm>

#include "GUIManager.h"
#include "skengio/utils/textureManager.h"

namespace SKEngio {

    class GUIManager;
    
    Renderer::Renderer(WindowManager* winMan) {
        //keeps a reference to winManager
        this->winMan = winMan;
        
        //create the render params object with defaults
        renderParams = new RenderParams();

        sceneStack = new SceneStack();

        //initializes the opengl stuff
        this->InitGL();

        //initialize the GUI system
        this->InitGUI();
    }

    Renderer::~Renderer() {
        delete sceneStack;
        delete guiMan;
        delete camera;
        delete fboShader;
        delete depthDebugShader;
        delete ShadowMap_Texture;
    }

    void Renderer::HandleResize(int width, int height) {
        
        for(Scene* scene : sceneStack->scenes) {
            scene->handleResize(width, height);
        }

        //buggy as fuck 
        //GenerateFrameBO(width, height);

    }

    void Renderer::OnEvent(Event* e) {

        //handle event by renderer
        if(e->type == EVENT_TYPE_RESIZE) {
            HandleResize(e->width, e->height);
        }

        if (e->type == EVENT_TYPE_KEYPRESS) {
            if (e->key == GLFW_KEY_U) {
                renderParams->drawUI = !renderParams->drawUI;
            }
        }

        //pass the event each scene
        for(Scene* scene : sceneStack->scenes) {
            scene->OnEvent(e);
        }

        //forward to GUI
        guiMan->OnEvent(e);
    }


    void Renderer::InitGUI() {
        guiMan = new SKEngio::GUIManager( this );
        guiMan->InitGUI(winMan->window);
    }


    bool Renderer::InitGL() {

        glfwMakeContextCurrent(this->winMan->window);

        //set vsync true
        glfwSwapInterval(1);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
        glfwWindowHint(GLFW_SAMPLES, 4);
        glEnable(GL_MULTISAMPLE);

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

        float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };

        // Create buffer: for the vertex data for frame buffer rectange
        glGenVertexArrays(1, &quad_VAO);
        glGenBuffers(1, &quad_VBO);
        glBindVertexArray(quad_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        GenerateFrameBO(winMan->width, winMan->height);
        GenerateDepthMapBuffers(winMan->width, winMan->height);
        GenerateShadowMapsBuffers();

        return true;   
    }

    void Renderer::GenerateDepthMapBuffers(int width, int height) {
        if (DepthMap_FBO != -1)
            glDeleteFramebuffers(1, &DepthMap_FBO);

        glGenFramebuffers(1, &DepthMap_FBO);

        if (DepthMap_Texture != NULL)
            glDeleteTextures(1, &DepthMap_Texture->textureID);


        DepthMap_Texture = TextureManager::getInstance()->CreateShadowMapTexture(width, height);

        //thell the FBO where to write
        glBindFramebuffer(GL_FRAMEBUFFER, DepthMap_FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthMap_Texture->textureID, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //shader for debugging depth
        depthDebugShader = new ShaderProgram();
        depthDebugShader->LoadShader("./shaders/", "depthmapDebug.vert", SKEngio::ShaderProgram::VERTEX);
        depthDebugShader->LoadShader("./shaders/", "depthmapDebug.frag", SKEngio::ShaderProgram::FRAGMENT);
        depthDebugShader->CreateProgram();

        depthDebugShader->SetDepthTexture(DepthMap_Texture->textureUnit);

    }


    void Renderer::GenerateShadowMapsBuffers() {
        if (ShadowMap_FBO != -1)
            glDeleteFramebuffers(1, &ShadowMap_FBO);

        glGenFramebuffers(1, &ShadowMap_FBO);

        if (ShadowMap_Texture != NULL)
            glDeleteTextures(1, &ShadowMap_Texture->textureID);


        ShadowMap_Texture = TextureManager::getInstance()->CreateShadowMapTexture(SHADOW_WIDTH, SHADOW_HEIGHT);

        //thell the FBO where to write
        glBindFramebuffer(GL_FRAMEBUFFER, ShadowMap_FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ShadowMap_Texture->textureID, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //shader for debugging depth
        shadowDebugShader = new ShaderProgram();
        shadowDebugShader->LoadShader("./shaders/", "depthmapDebug.vert", SKEngio::ShaderProgram::VERTEX);
        shadowDebugShader->LoadShader("./shaders/", "depthmapDebug.frag", SKEngio::ShaderProgram::FRAGMENT);
        shadowDebugShader->CreateProgram();

        shadowDebugShader->SetDepthTexture(ShadowMap_Texture->textureUnit);

    }

    void Renderer::GenerateFrameBO(unsigned int width, unsigned int height) {
        if(Post_FBOtexture != NULL)
            glDeleteTextures(1, &Post_FBOtexture->textureID);


        Post_FBOtexture = TextureManager::getInstance()->CreateFrameBufferTexture(winMan->width, winMan->height);

        //create the texture for framebuffer
        //glGenTextures(1, &Post_FBOtexture);
        //glBindTexture(GL_TEXTURE_2D, Post_FBOtexture);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, winMan->width, winMan->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        if (Post_FBO != -1)
            glDeleteFramebuffers(1, &Post_FBO);

        //frame buffer object for fx
        glGenFramebuffers(1, &Post_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, Post_FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Post_FBOtexture->textureID, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            SK_LOG_ERR( "ERROR Creating Post Frame Buffer Object");
        }

        if (Post_RBO != -1)
            glDeleteRenderbuffers(1, &Post_RBO);

        // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
        glGenRenderbuffers(1, &Post_RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, Post_RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, Post_RBO); // now actually attach it

        //shader for FBO
        fboShader = new ShaderProgram();
        fboShader->LoadShader("./shaders/", "fboshader.vert", SKEngio::ShaderProgram::VERTEX);
        fboShader->LoadShader("./shaders/", "fboshader.frag", SKEngio::ShaderProgram::FRAGMENT);
        fboShader->CreateProgram();

    }

    void Renderer::_DrawUI() {
        guiMan->DrawBegin();
        guiMan->Draw();
        for (Scene* scene : sceneStack->scenes) {
            scene->OnDrawGUI(renderParams);
        }
        guiMan->DrawEnd(winMan->window);
    }

    void Renderer::_ShadowMapPass() {

        /*
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 7.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;

        // render scene from light's point of view
        simpleDepthShader.use();
        simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        */
        
        RenderPass curPass = renderParams->pass;
        renderParams->pass = RenderPass::ShadowDepth;

        //glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, DepthMap_FBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        DepthMap_Texture->bind();
        //update and render all scenes
        for (Scene* scene : sceneStack->scenes) {
            //TODO: should manage double update per frame in case of shadowPass
            scene->UpdateAndDraw(renderParams);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        DepthMap_Texture->unbind();
        renderParams->pass = curPass;
    }

    void Renderer::Draw() {

        if (winMan->width == 0 || winMan->height == 0)
            return;

        renderParams->time = glfwGetTime();

        if(renderParams->useShadows)
            _ShadowMapPass();

        //enable the frame buffer object
        glBindFramebuffer(GL_FRAMEBUFFER, Post_FBO);
        
        camera->UpdateViewport();

        //deafult rendering settings
        glDepthMask(GL_TRUE);
        glClearDepth(1.0);
        glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        if (depthDebug) {

            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
            glEnable(GL_TEXTURE_2D);
            depthDebugShader->bind();
            DepthMap_Texture->bind();

            glBindVertexArray(quad_VAO); // VAO of the quad
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            depthDebugShader->unbind();
            DepthMap_Texture->unbind();

        }
        else 
        {

            //update and render all scenes
            for (Scene* scene : sceneStack->scenes) {
                scene->UpdateAndDraw(renderParams);
            }

            //disable the frame buffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            //begin the gui rendering
            if (renderParams->drawUI) 
                _DrawUI();

            //---BEGIN rendering frame buffer quad
            //frame buffer quads settings
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
            glEnable(GL_TEXTURE_2D);
            fboShader->bind();
            Post_FBOtexture->bind();

            glBindVertexArray(quad_VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            fboShader->unbind();
            Post_FBOtexture->unbind();
            //---END rendering frame buffer quad

            GLenum err = glGetError();
            if (err != 0) {
                SK_LOG_ERR("GLError CODE: " << err );
            }

            //end the gui rendering
            if (renderParams->drawUI) 
                guiMan->DrawSwapBuffers();
        }

        glfwMakeContextCurrent(winMan->window);
        glfwSwapBuffers(winMan->window);
    }

    Camera* Renderer::NewCamera(float fov, std::string camID) {
        camera = new Camera(winMan->width, winMan->height, fov, camID);
        return camera;
    }

    void Renderer::AddScene(Scene* newScene) {
        if (camera == NULL) {
            SK_LOG_ERR("ERROR! Adding scene to Renderer before adding a camera");
        }
        newScene->setActiveCamera(camera);
        sceneStack->AddScene(newScene);
    }

    SceneStack* Renderer::GetSceneStack() {
        return sceneStack;
    }

}

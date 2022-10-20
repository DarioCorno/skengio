#include "renderer.h"

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <algorithm>

#include "GUIManager.h"
#include "skengio/utils/textureManager.h"
#include "utils/shaderProgram.h"

namespace SKEngio {

    class GUIManager;
    
    Renderer::Renderer(WindowManager* winMan) {
        //keeps a reference to winManager
        this->winMan = winMan;
        
        //create the render params object with defaults
        renderParams = std::make_unique<RenderParams>();

        sceneStack = std::make_unique<SceneStack>();

        //initializes the opengl stuff
        this->InitGL();

        //initialize the GUI system
        this->InitGUI();
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
        if(e->type == EventType::Resize) {
            HandleResize(e->width, e->height);
        }

        if (e->type == EventType::KeyPress) {
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
        guiMan = std::make_unique<GUIManager>( this );
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
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        GenerateFrameBO(winMan->width, winMan->height);
        //GenerateDepthMapBuffers(winMan->width, winMan->height);
        //GenerateShadowMapsBuffers();

        return true;   
    }

    void Renderer::GenerateDepthMapBuffers(int width, int height) {
        //glGenFramebuffers(1, &DepthMap_FBO);
        //
        //DepthMap_Texture = TextureManager::getInstance().CreateShadowMapTexture(width, height);
        //
        ////thell the FBO where to write
        //glBindFramebuffer(GL_FRAMEBUFFER, DepthMap_FBO);
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthMap_Texture->textureID, 0);
        //glDrawBuffer(GL_NONE);
        //glReadBuffer(GL_NONE);
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //
        ////shader for debugging depth
        //depthDebugShader = std::make_unique<ShaderProgram>();
        //depthDebugShader->LoadShader("./shaders/", "depthmapDebug.vert", SKEngio::ShaderProgram::VERTEX);
        //depthDebugShader->LoadShader("./shaders/", "depthmapDebug.frag", SKEngio::ShaderProgram::FRAGMENT);
        //depthDebugShader->CreateProgram();
        //
        //depthDebugShader->SetDepthTexture(DepthMap_Texture->textureUnit);
        //
        //renderParams->depthMap = DepthMap_Texture;

    }


    void Renderer::GenerateShadowMapsBuffers() {
        glGenFramebuffers(1, &ShadowMap_FBO);

        if (ShadowMap_Texture != nullptr)
            glDeleteTextures(1, &ShadowMap_Texture->textureID);


        ShadowMap_Texture = TextureManager::getInstance().CreateShadowMapTexture(SHADOW_WIDTH, SHADOW_HEIGHT);

        //thell the FBO where to write
        glBindFramebuffer(GL_FRAMEBUFFER, ShadowMap_FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ShadowMap_Texture->textureID, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //shader for debugging depth
        shadowDebugShader = std::make_unique<ShaderProgram>();
        shadowDebugShader->LoadShader("./shaders/", "depthmapDebug.vert", SKEngio::ShaderProgram::VERTEX);
        shadowDebugShader->LoadShader("./shaders/", "depthmapDebug.frag", SKEngio::ShaderProgram::FRAGMENT);
        shadowDebugShader->CreateProgram();

        shadowDebugShader->SetDepthTexture(ShadowMap_Texture->textureUnit);

    }

    void Renderer::GenerateFrameBO(unsigned int width, unsigned int height) {

        if(FrameBOtexture != nullptr)
            glDeleteTextures(1, &FrameBOtexture->textureID);

        //frame buffer object for fx ("container" of many render buffers
        glGenFramebuffers(1, &FrameBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FrameBO);

       FrameBOtexture = TextureManager::getInstance().CreateFrameBufferTexture(winMan->width, winMan->height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FrameBOtexture->textureID, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            SK_LOG_ERR( "ERROR Creating Post Frame Buffer Object");
        }

        //shader for FrameBO
        fboShader = std::make_unique<ShaderProgram>();
        fboShader->LoadShader("./shaders/", "fboshader.vert", SKEngio::ShaderProgram::VERTEX);
        fboShader->LoadShader("./shaders/", "fboshader.frag", SKEngio::ShaderProgram::FRAGMENT);
        fboShader->CreateProgram();

        // create a renderbuffer object for depth attachment
        glGenRenderbuffers(1, &DepthRBO);
        glBindRenderbuffer(GL_RENDERBUFFER, DepthRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
        //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, FrameBO); // attach it to FBO

        // attach the renderbuffer to depth attachment point
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
            GL_DEPTH_ATTACHMENT, // 2. attachment point
            GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
            DepthRBO);              // 4. rbo ID
        
        DepthBOTexture = TextureManager::getInstance().CreateShadowMapTexture(winMan->width, winMan->height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthBOTexture->textureID, 0);

        //
        //if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        //    SK_LOG_ERR("ERROR Creating Depth Render Buffer Object");
        //}
        ////qui va messa la creazione del depth buffer
        ////https ://stackoverflow.com/questions/2213030/whats-the-concept-of-and-differences-between-framebuffer-and-renderbuffer-in-op
        ////http://www.songho.ca/opengl/gl_fbo.html
        
        depthDebugShader = std::make_unique<ShaderProgram>();
        depthDebugShader->LoadShader("./shaders/", "depthmapDebug.vert", SKEngio::ShaderProgram::VERTEX);
        depthDebugShader->LoadShader("./shaders/", "depthmapDebug.frag", SKEngio::ShaderProgram::FRAGMENT);
        depthDebugShader->CreateProgram();
        depthDebugShader->SetDepthTexture(DepthBOTexture->textureUnit);
        //renderParams->depthMap = DepthMap_Texture;
        

    }

    void Renderer::DrawUI() {
        guiMan->DrawBegin();
        guiMan->Draw();
        for (Scene* scene : sceneStack->scenes) {
            scene->OnDrawGUI(renderParams.get());
        }
        guiMan->DrawEnd(winMan->window);
    }

    void Renderer::ShadowMapPass() {

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
        glBindFramebuffer(GL_FRAMEBUFFER, FrameBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        DepthBOTexture->bind();
        //update and render all scenes
        for (Scene* scene : sceneStack->scenes) {
            //TODO: should manage double update per frame in case of shadowPass
            scene->UpdateAndDraw(renderParams.get());
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        DepthBOTexture->unbind();
        renderParams->pass = curPass;
    }

    void Renderer::Draw() {

        if (winMan->width == 0 || winMan->height == 0)
            return;

        renderParams->time = glfwGetTime();

        if(renderParams->useShadows)
            ShadowMapPass();

        //enable the frame buffer object
        glBindFramebuffer(GL_FRAMEBUFFER, FrameBO);
        
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


        //update and render all scenes
        for (Scene* scene : sceneStack->scenes) {
            scene->UpdateAndDraw(renderParams.get());
        }

        //disable the frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //begin the gui rendering
        if (renderParams->drawUI) 
            DrawUI();

        //---BEGIN rendering frame buffer quad
        //frame buffer quads settings
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glEnable(GL_TEXTURE_2D);
        if (!depthDebug) {
            fboShader->bind();
            FrameBOtexture->bind();
        }
        else {
            depthDebugShader->bind();
            DepthBOTexture->bind();
        }

        glBindVertexArray(quad_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        if (!depthDebug) {
            fboShader->unbind();
            FrameBOtexture->unbind();
        }
        else {
            depthDebugShader->unbind();
            DepthBOTexture->unbind();
        }
        //---END rendering frame buffer quad

        GLenum err = glGetError();
        if (err != 0) {
            SK_LOG_ERR("GLError CODE: " << err );
        }

        //end the gui rendering
        if (renderParams->drawUI) 
            guiMan->DrawSwapBuffers();

        glfwMakeContextCurrent(winMan->window);
        glfwSwapBuffers(winMan->window);
    }

    void Renderer::NewCamera(float fov, std::string camID) {
        camera = std::make_unique<Camera>(winMan->width, winMan->height, fov, std::move(camID));
    }

    void Renderer::AddScene(Scene* newScene) {
        if (camera == nullptr) {
            SK_LOG_ERR("ERROR! Adding scene to Renderer before adding a camera");
        }
        newScene->setActiveCamera(camera.get());
        sceneStack->AddScene(newScene);
    }

    SceneStack* Renderer::GetSceneStack() {
        return sceneStack.get();
    }

}

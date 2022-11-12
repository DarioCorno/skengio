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
    
    void Renderer::Init() {
        //keeps a reference to winManager
        //this->winMan = winMan;

        //create the render params object with defaults
        renderParams = std::make_unique<RenderParams>();

        sceneStack = std::make_unique<SceneStack>();

        //initializes the opengl stuff
        InitGL();

        //initialize the GUI system
        GUIManager::get().InitGUI();
    }

    void Renderer::OnDestroy() {
        sceneStack->OnDestroy();
        gizmoShader->OnDestroy();
        fboShader->OnDestroy();
        depthDebugShader->OnDestroy();

        glDeleteFramebuffers(1, &Final_FBO);
        glDeleteFramebuffers(1, &Final_FBO);
        glDeleteFramebuffers(1, &ShadowMap_FBO);
    }

    void Renderer::HandleResize(int width, int height) {
        
        for(Scene* scene : sceneStack->scenes) {
            scene->handleResize(width, height);
        }

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
        GUIManager::get().OnEvent(e);
    }


    bool Renderer::InitGL() {

        glfwMakeContextCurrent(WindowManager::get().window);

        //set vsync true
        glfwSwapInterval(0);

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

        InitFulscreenQuad();
        InitDebugQuad();

        GenerateFrameBO(WindowManager::get().width, WindowManager::get().height);
        GenerateShadowMapsBO();
        //used to render objects from light point of view
        LoadShadowMapShader();
        GenerateGizmosShader();

        return true;   
    }

    void Renderer::GenerateGizmosShader() {
        gizmoShader = std::make_unique<ShaderProgram>();
        gizmoShader->LoadShader("./shaders/", "utility.vert", SKEngio::ShaderProgram::VERTEX);
        gizmoShader->LoadShader("./shaders/", "utility.frag", SKEngio::ShaderProgram::FRAGMENT);
        gizmoShader->CreateProgram();
    }

    ShaderProgram* Renderer::GizmoGetShader() {
        return gizmoShader.get();
    }

    void Renderer::InitFulscreenQuad() {
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

    }

    void Renderer::InitDebugQuad() {
        float debugVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
            -1.0f,  -0.5,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             -0.5, -1.0f,  1.0f, 0.0f,

            -1.0f,  -0.5,  0.0f, 1.0f,
             -0.5, -1.0f,  1.0f, 0.0f,
             -0.5,  -0.5,  1.0f, 1.0f
        };

        // Create buffer: for the vertex data for frame buffer rectange
        glGenVertexArrays(1, &debug_VAO);
        glGenBuffers(1, &debug_VBO);
        glBindVertexArray(debug_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, debug_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(debugVertices), &debugVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    }

    void Renderer::LoadShadowMapShader() {
        //shader for rendering depth shadows (uses light matrix instead of camera ones)
        //this debug is done with depthmapDebug
        shadowMapShader = new ShaderProgram();
        shadowMapShader->LoadShader("./shaders/", "shadowMap.vert", SKEngio::ShaderProgram::VERTEX);
        shadowMapShader->LoadShader("./shaders/", "shadowMap.frag", SKEngio::ShaderProgram::FRAGMENT);
        shadowMapShader->CreateProgram();
    }

    void Renderer::GenerateShadowMapsBO() {
    
        glGenFramebuffers(1, &ShadowMap_FBO);
    
    }

    void Renderer::GenerateFrameBO(unsigned int width, unsigned int height) {

        if(FrameBOtexture != nullptr)
            glDeleteTextures(1, &FrameBOtexture->textureID);

        //frame buffer object for fx ("container" of many render buffers
        glGenFramebuffers(1, &Final_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, Final_FBO);

        FrameBOtexture = TextureManager::get().CreateFrameBufferTexture(WindowManager::get().width, WindowManager::get().height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FrameBOtexture->textureID, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            SK_LOG_ERR( "ERROR Creating Frame Buffer Object");
        }

        //shader for FrameBO
        fboShader = std::make_unique<ShaderProgram>();
        fboShader->LoadShader("./shaders/", "fboshader.vert", SKEngio::ShaderProgram::VERTEX);
        fboShader->LoadShader("./shaders/", "fboshader.frag", SKEngio::ShaderProgram::FRAGMENT);
        fboShader->CreateProgram();

        // create a renderbuffer object for depth attachment
        glGenRenderbuffers(1, &DepthRBO);
        glBindRenderbuffer(GL_RENDERBUFFER, DepthRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height); 

        // attach the renderbuffer to depth attachment point
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
            GL_DEPTH_ATTACHMENT, // 2. attachment point
            GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
            DepthRBO);              // 4. rbo ID
        
        DepthBOTexture = TextureManager::get().CreateShadowMapTexture( WindowManager::get().width, WindowManager::get().height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthBOTexture->textureID, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            SK_LOG_ERR("ERROR Creating Depth Render Buffer Object");
        }
        
        //only used for rendering the debug quad
        depthDebugShader = std::make_unique<ShaderProgram>();
        depthDebugShader->LoadShader("./shaders/", "depthmapDebug.vert", SKEngio::ShaderProgram::VERTEX);
        depthDebugShader->LoadShader("./shaders/", "depthmapDebug.frag", SKEngio::ShaderProgram::FRAGMENT);
        depthDebugShader->CreateProgram();
        depthDebugShader->SetDepthTexture(DepthBOTexture->textureUnit);

        //bind the depth texture to the main fbo shader (for DOF effect for example)
        fboShader->SetDepthTexture(DepthBOTexture->textureUnit);
        //renderParams->depthMap = DepthMap_Texture;
        

    }

    void Renderer::DrawUI() {
        GUIManager::get().DrawBegin();
        GUIManager::get().Draw();
        for (Scene* scene : sceneStack->scenes) {
            scene->OnDrawGUI(renderParams.get());
        }
        GUIManager::get().DrawEnd();
    }

    void Renderer::SetCurrentScene(RenderParams* rp) {
        //will set the current scene according to time and other params, now scene 0 for testing
        scene = sceneStack->scenes[0];    
    }

    unsigned int Renderer::GetShadowMapFBOID() {
        return ShadowMap_FBO;
    }

    void Renderer::ShadowMapPass() {

        glBindFramebuffer(GL_FRAMEBUFFER, ShadowMap_FBO);

        for (Light* light : scene->lights) {

            if (light->castShadows && light->enabled) {

                //bind the generic depth buffer shader (objects are rendered with a super simple shader)
                shadowMapShader->bind();

                //set the light projection in the generic depth shader
                shadowMapShader->SetLightUniforms(light->GetPosition(), light->GetDiffuse(), light->getDirLightViewProjMatrix());

                //bind buffers, set viewport and other params
                light->BeginShadowMapRender();

                //set the light's shadow shader (needs to render depth only objects)
                //this is identical for all shadow renderers, so it could be a renderer class member
                renderParams->passShader = shadowMapShader;

                //render all scenes
                for (Scene* scene : sceneStack->scenes) {
                    scene->OnDraw(renderParams.get());
                }

                light->EndShadowMapRender();

            }

        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Renderer::Draw() {

        if (WindowManager::get().width == 0 || WindowManager::get().height == 0)
            return;
        
        //update current frame time and render params
        renderParams->NewFrame();

        //retrieves the current scene according to timeline (to be implemented)
        SetCurrentScene( renderParams.get() );

        //update current scene
        renderParams->camera = scene->camera;
        scene->OnUpdate(renderParams.get());

        //this must be done AFTER scene->update (now the rendering happens twice)
        renderParams->pass = RenderPass::ShadowDepth;
        glCullFace(GL_FRONT);
        ShadowMapPass();
        glCullFace(GL_BACK);
        renderParams->pass = RenderPass::Final;

        //enable the final frame buffer object
        glBindFramebuffer(GL_FRAMEBUFFER, Final_FBO);

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


        //--- RENDER active scene
        //set the camera back, maybe we did some different projections
        scene->camera->UpdateViewport();
        renderParams->camera = scene->camera;
        //scene->OnUpdate( renderParams.get() );
        scene->OnDraw( renderParams.get());

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

        fboShader->bind();
        fboShader->SetCameraUniforms(scene->camera);       //set the camera data into fbo shader
        //FrameBOtexture->bind();

        glBindVertexArray(quad_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        fboShader->unbind();
        //FrameBOtexture->unbind();
        //---END rendering frame buffer quad


        if (depthDebug) {
            depthDebugShader->SetDepthTexture(DepthBOTexture->textureUnit);
            depthDebugShader->SetCameraUniforms(scene->camera);       //set the camera data into depth rbo shader
            depthDebugShader->bind();
            //DepthBOTexture->bind();

            glBindVertexArray(debug_VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            depthDebugShader->unbind();
            //DepthBOTexture->unbind();
        }

        if (shadowsDebug) {
            Light* light = scene->lights[0];
            depthDebugShader->SetDepthTexture(light->GetShadowTexture()->textureUnit);
            depthDebugShader->SetCameraUniforms(scene->camera);       //set the camera data into depth rbo shader
            //this must be scene bounding box
            depthDebugShader->SetFarNearUniforms(0.1f, 100.0f);
            depthDebugShader->bind();
            //ShadowMap_Texture->bind();

            glBindVertexArray(debug_VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            depthDebugShader->unbind();
            //ShadowMap_Texture->unbind();
        }

        GLenum err = glGetError();
        if (err != 0) {
            SK_LOG_ERR("GLError CODE: " << err );
        }

        //end the gui rendering
        if (renderParams->drawUI) 
            GUIManager::get().DrawSwapBuffers();

        glfwMakeContextCurrent(WindowManager::get().window);
        glfwSwapBuffers(WindowManager::get().window);
    }

    void Renderer::AddScene(Scene* newScene) {
        sceneStack->AddScene(newScene);
    }

    SceneStack* Renderer::GetSceneStack() {
        return sceneStack.get();
    }

}

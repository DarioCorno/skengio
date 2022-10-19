#pragma once

#ifndef _SK_RENDERER_
#define _SK_RENDERER_

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "scene.h"
#include "scenestack.h"
#include "renderParams.h"
#include "window.h"

namespace SKEngio {

    class GUIManager;
    class ShaderProgram;

    class Renderer {
        public:

            Renderer(WindowManager* winMan);

            virtual ~Renderer();

            void Render();

            bool InitGL();

            void InitGUI();

            void Draw();

            void OnEvent(Event* e);

            SceneStack* GetSceneStack();

            void AddScene(Scene* newScene);

            Camera* NewCamera(float fov, std::string camID);

            SKEngio::RenderParams* renderParams;

        private:

            const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

            void _DrawUI();
            void _ShadowMapPass();
            void GenerateFrameBO(unsigned int width, unsigned int height);
            void GenerateShadowMapsBuffers();
            void HandleResize(int width, int height);
            void setPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);

            //just a reference (shared with Application)
            WindowManager* winMan;

            //the gui manager
            GUIManager* guiMan;

        	glm::mat4x4 mProjMatrix, mModelViewMatrix;

            SceneStack* sceneStack;

            bool depthDebug = false;

            unsigned int quad_VBO{};
            unsigned int quad_VAO{};

            unsigned int Post_FBO = -1;
            unsigned int Post_RBO = -1;
            Texture* Post_FBOtexture = NULL;

            unsigned int Depth_FBO{};
            Texture* Depth_Texture = NULL;

            SKEngio::ShaderProgram* fboShader;
            SKEngio::ShaderProgram* depthDebugShader;

            Camera* camera = NULL;

    };
}
#endif
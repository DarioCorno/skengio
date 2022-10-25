#pragma once

#ifndef SK_RENDERER_
#define SK_RENDERER_

#include "GUIManager.h"
#include "glm/glm.hpp"

#include "scene.h"
#include "scenestack.h"
#include "renderParams.h"
#include "window.h"
#include "utils/shaderProgram.h"
#include "utils/texture.h"
#include "skengio/utils/singleton.h"

namespace SKEngio {
    class Renderer final : public Singleton<Renderer>{
        public:

            //Renderer(WindowManager* winMan);

            void Init();

            bool InitGL();

            void Draw();

            void OnEvent(Event* e);

            SceneStack* GetSceneStack();

            void AddScene(Scene* newScene);

            std::unique_ptr<RenderParams> renderParams;

            bool depthDebug = false;

        private:

            const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

            void InitFulscreenQuad();
            void InitDebugQuad();
            void DrawUI();
            void ShadowMapPass();
            void GenerateFrameBO(unsigned int width, unsigned int height);
            void GenerateShadowMapsBuffers();
            void HandleResize(int width, int height);
            void UpdateCurrentScene(RenderParams* rp);
            //void SetPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);

        	glm::mat4x4 mProjMatrix, mModelViewMatrix;

            std::unique_ptr<SceneStack> sceneStack;
            Scene* scene;   //current active scene

            unsigned int quad_VBO{};
            unsigned int quad_VAO{};

            unsigned int debug_VBO{};
            unsigned int debug_VAO{};

            //frame buffer object
            unsigned int FrameBO = -1;
            Texture* FrameBOtexture;
            std::unique_ptr<ShaderProgram> fboShader;

            //render buffer for depth
            unsigned int DepthRBO{};
            Texture* DepthBOTexture;
            std::unique_ptr<ShaderProgram> depthDebugShader;

            unsigned int ShadowMap_FBO{};
            Texture* ShadowMap_Texture;
            std::unique_ptr<ShaderProgram> shadowDebugShader;

    };
}
#endif
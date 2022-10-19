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

namespace SKEngio {
    class Renderer final {
        public:

            Renderer(WindowManager* winMan);

            bool InitGL();

            void InitGUI();

            void Draw();

            void OnEvent(Event* e);

            SceneStack* GetSceneStack();

            void AddScene(Scene* newScene);

            void NewCamera(float fov, std::string camID);

            std::unique_ptr<RenderParams> renderParams;

        private:

            const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

            void DrawUI();
            void ShadowMapPass();
            void GenerateFrameBO(unsigned int width, unsigned int height);
            void GenerateShadowMapsBuffers();
            void GenerateDepthMapBuffers(int width, int height);
            void HandleResize(int width, int height);
            //void SetPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);

            //just a reference (shared with Application)
            WindowManager* winMan;

            //the gui manager
            std::unique_ptr<GUIManager> guiMan;

        	glm::mat4x4 mProjMatrix, mModelViewMatrix;

            std::unique_ptr<SceneStack> sceneStack;

            bool depthDebug = false;

            unsigned int quad_VBO{};
            unsigned int quad_VAO{};

            unsigned int Post_FBO = -1;
            unsigned int Post_RBO = -1;
            std::unique_ptr<Texture> Post_FBOtexture;
            std::unique_ptr<ShaderProgram> fboShader;

            unsigned int ShadowMap_FBO{};
            std::unique_ptr<Texture> ShadowMap_Texture;
            std::unique_ptr<ShaderProgram> shadowDebugShader;

            unsigned int DepthMap_FBO{};
            std::unique_ptr<Texture> DepthMap_Texture;
            std::unique_ptr<ShaderProgram> depthDebugShader;

            std::unique_ptr<Camera> camera = nullptr;

    };
}
#endif
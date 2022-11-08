#pragma once

#ifndef SK_RENDERPARAMS_
#define SK_RENDERPARAMS_

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "utils/texture.h"
//#include "skengio/entities/light.h"
#include "skengio/utils/shaderprogram.h"

namespace SKEngio {

    enum class RenderPass {
        CameraDepth = 0,
        ShadowDepth,
        Final
    };

    class RenderParams {
    public:
        RenderPass pass = RenderPass::Final;
        bool drawUI = true;

        float time;
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;

        Texture* depthMap;
        Camera* camera;
        //Light* light;
        ShaderProgram* passShader;

        void NewFrame() {
            time = static_cast<float>(glfwGetTime());
            deltaTime = time - lastFrame;
            lastFrame = time;
        };

    };
}

#endif
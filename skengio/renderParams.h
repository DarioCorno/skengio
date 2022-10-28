#pragma once

#ifndef SK_RENDERPARAMS_
#define SK_RENDERPARAMS_

#include "utils/texture.h"
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
        Texture* depthMap;
        Camera* camera;
        ShaderProgram* passShader;
    };
}

#endif
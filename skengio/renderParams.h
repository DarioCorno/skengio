#pragma once

#ifndef SK_RENDERPARAMS_
#define SK_RENDERPARAMS_

#include "utils/texture.h"

namespace SKEngio {

    enum class RenderPass {
        CameraDepth = 0,
        ShadowDepth,
        Final
    };

    class RenderParams {
    public:
        RenderPass pass = RenderPass::Final;
        bool useShadows = false;
        bool drawUI = true;
        float time;
        Texture* depthMap;
        Camera* camera;
    };
}

#endif
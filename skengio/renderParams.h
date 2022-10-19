#pragma once

#ifndef SK_RENDERPARAMS_
#define SK_RENDERPARAMS_

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
    };
}

#endif
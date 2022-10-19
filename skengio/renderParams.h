#pragma once

#ifndef _SK_RENDERPARAMS_
#define _SK_RENDERPARAMS_

namespace SKEngio {

    enum RenderPass {
        CameraDepth = 0,
        ShadowDepth,
        Final
    };

    class RenderParams {
    public:
        RenderParams() {
            pass = RenderPass::Final;
            useShadows = false;
            drawUI = true;
        }

        ~RenderParams() {

        }

        RenderPass pass;
        bool useShadows;
        bool drawUI;
        float time;
    };
}

#endif
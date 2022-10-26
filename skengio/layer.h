#pragma once

#ifndef SK_LAYER_
#define SK_LAYER_

#include "event.h"
#include "camera.h"
#include "skengio/renderParams.h"

namespace SKEngio {

    class Layer {
        public:
        
        //Camera* activeCamera;
        
        Layer() = default;

        virtual ~Layer() = default;

        void SetId(unsigned int id);
        unsigned int GetId();


		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate(RenderParams* rp);
        virtual void OnDraw(RenderParams* rp);
        virtual void OnDrawGUI(RenderParams* rp);
        virtual void OnEvent(Event* e);

        //void setCamera(Camera* newCam) { activeCamera = newCam; }

        bool enabled = true;
        
        private:

        unsigned int id;

    };
}

#endif

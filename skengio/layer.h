#pragma once

#ifndef SK_LAYER_
#define SK_LAYER_

#include "event.h"
#include "camera.h"
#include <skengio/renderParams.h>
//#include <skengio/utils/object.h>

namespace SKEngio {

    class Layer {
        public:
        
        Camera* activeCamera;
        
        Layer() = default;

        // prevent copying object
        Layer(const Layer&) = delete;
        Layer(Layer&&) = delete;
        Layer& operator=(const Layer&) = delete;
        Layer& operator=(Layer&&) = delete;

        virtual ~Layer() = default;

        void SetId(unsigned int id);
        unsigned int GetId();


		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate(RenderParams* rp);
        virtual void OnDraw(RenderParams* rp);
        virtual void OnDrawGUI(RenderParams* rp);
        virtual void OnEvent(Event* e);

        //void AddObject(Object* obj);

        void setCamera(Camera* newCam) { activeCamera = newCam; }

        bool enabled = true;
        
        private:

        unsigned int id;

        //std::list<Object*> objects;

    };
}

#endif

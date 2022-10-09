#pragma once

#include "event.h"
#include "camera.h"

namespace SKEngio {
    class Layer {
        public:
        
        Camera* activeCamera;
        
        Layer();

        ~Layer();

        void SetId(unsigned int id);
        unsigned int GetId();


		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate(float time);
        virtual void OnDraw(float time);
        virtual void OnDrawGUI(float time);
        virtual void OnEvent(Event* e);

        void setCamera(Camera* newCam) { activeCamera = newCam; };
		//virtual void OnEvent(Event& event) {}        

        bool enabled = true;
        
        private:

        unsigned int id;

    };
}
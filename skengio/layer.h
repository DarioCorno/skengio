#pragma once

#include "event.h"

namespace SKEngio {
    class Layer {
        public:
        
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

		//virtual void OnEvent(Event& event) {}        

        private:

        unsigned int id;
    };
}
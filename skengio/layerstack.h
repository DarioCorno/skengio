#pragma once

#ifndef _SK_LAYERSTACK_
#define _SK_LAYERSTACK_

#include <vector>

#include "layer.h"
#include "event.h"

namespace SKEngio {

    class LayerStack {
        public:

        LayerStack();

        virtual ~LayerStack();

        void PushLayer(Layer* layer);

        void PopLayer(Layer* layer);

        void Destroy();

        void OnEvent(Event* e);


		std::vector<Layer*>::iterator begin() { return layers.begin(); }
		std::vector<Layer*>::iterator end() { return layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return layers.rend(); }        

		std::vector<Layer*>::const_iterator begin() const { return layers.begin(); }
		std::vector<Layer*>::const_iterator end()	const { return layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return layers.rend(); }

        std::vector<Layer*> layers;

        private:

        unsigned int layerInsertIndex = 0;
    };

}

#endif
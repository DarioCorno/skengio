#include "layer.h"
#include "layerstack.h"

#include <list>
#include <algorithm>
#include <iterator>
#include <iostream>

namespace SKEngio {

    LayerStack::LayerStack() {
		layers.clear();
    }

    LayerStack::~LayerStack() {

    }

    void LayerStack::PushLayer(Layer* layer) {
		layers.emplace(layers.begin() + layerInsertIndex, layer);
		layer->OnAttach();
		layerInsertIndex++;
    }

	void LayerStack::PopLayer(Layer* layer) {

		auto it = std::find(layers.begin(), layers.begin() + layerInsertIndex, layer);
		if (it != layers.begin() + layerInsertIndex)
		{
			layer->OnDetach();
			layers.erase(it);
			layerInsertIndex--;
		}
	}   

	void LayerStack::OnEvent(Event* e) {
		//manage event for layerstack

		//pass the event to all the layers
		for(Layer* layer : layers){
			layer->OnEvent(e);
		}
	}


	void LayerStack::Destroy() {
		for(Layer* layer : layers) {
			layer->OnDetach();
		}

		layers.clear();
	} 
}
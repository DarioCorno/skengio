#include "layer.h"
#include "layerstack.h"

namespace SKEngio {
    void LayerStack::PushLayer(Layer* layer) {
		layers.push_back(layer);
		layer->OnAttach();
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
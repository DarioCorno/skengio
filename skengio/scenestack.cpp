#include "scene.h"
#include "scenestack.h"

namespace SKEngio {

    SceneStack::~SceneStack() {
		for (Scene* scene : scenes) {
			scene->OnDetach();
		}

		scenes.clear();
    }

    void SceneStack::AddScene(Scene* scene) {
		scenes.push_back(scene);
		scene->OnAttach();
    }

	void SceneStack::OnEvent(Event* e) {
		//manage event for layerstack

		//pass the event to all the layers
		for(Scene* scene : scenes){
			scene->OnEvent(e);
		}
	}
}
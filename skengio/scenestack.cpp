#include "scene.h"
#include "scenestack.h"

namespace SKEngio {

	SceneStack::~SceneStack() {

	}

    void SceneStack::OnDestroy() {
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

		//pass the event to all the layers
		for(Scene* scene : scenes){
			scene->OnEvent(e);
		}
	}
}
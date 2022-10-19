#include "scene.h"
#include "scenestack.h"

#include <list>
#include <algorithm>
#include <iterator>
#include <iostream>

namespace SKEngio {

    SceneStack::SceneStack() {
		scenes.clear();
    }

    SceneStack::~SceneStack() {
		for (Scene* scene : scenes) {
			scene->OnDetach();
		}

		scenes.clear();
    }

    void SceneStack::AddScene(Scene* scene) {
		scenes.emplace(scenes.begin() + sceneInsertIndex, scene);
		scene->OnAttach();
		sceneInsertIndex++;
    }

	void SceneStack::PopScene(Scene* scene) {
	
		auto it = std::find(scenes.begin(), scenes.begin() + sceneInsertIndex, scene);
		if (it != scenes.begin() + sceneInsertIndex)
		{
			scene->OnDetach();
			scenes.erase(it);
			sceneInsertIndex--;
		}
	}   

	void SceneStack::OnEvent(Event* e) {
		//manage event for layerstack

		//pass the event to all the layers
		for(Scene* scene : scenes){
			scene->OnEvent(e);
		}
	}


}
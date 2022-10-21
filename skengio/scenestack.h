#pragma once

#ifndef SK_SCENESTACK_
#define SK_SCENESTACK_

#include <vector>

#include "event.h"
#include "scene.h"

namespace SKEngio {

    class SceneStack final {
    public:
        SceneStack() = default;
        
        // prevent copying object
        SceneStack(const SceneStack&) = delete;
        SceneStack(SceneStack&&) = delete;
        SceneStack& operator=(const SceneStack&) = delete;
        SceneStack& operator=(SceneStack&&) = delete;

        ~SceneStack();

        void AddScene(Scene* scene);

        void OnEvent(Event* e);
        
		std::vector<Scene*>::iterator begin() { return scenes.begin(); }
		std::vector<Scene*>::iterator end() { return scenes.end(); }
		std::vector<Scene*>::reverse_iterator rbegin() { return scenes.rbegin(); }
		std::vector<Scene*>::reverse_iterator rend() { return scenes.rend(); }        

		std::vector<Scene*>::const_iterator begin() const { return scenes.begin(); }
		std::vector<Scene*>::const_iterator end()	const { return scenes.end(); }
		std::vector<Scene*>::const_reverse_iterator rbegin() const { return scenes.rbegin(); }
		std::vector<Scene*>::const_reverse_iterator rend() const { return scenes.rend(); }

        std::vector<Scene*> scenes;

    private:
    };

}

#endif
#pragma once

#include <vector>

#include "scene.h"
#include "event.h"

namespace SKEngio {

    class SceneStack {
        public:

        SceneStack();

        virtual ~SceneStack();

        void AddScene(Scene* scene);

        void PopScene(Scene* scene);

        void Destroy();

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

        unsigned int sceneInsertIndex = 0;
    };

}
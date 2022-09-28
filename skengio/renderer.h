#pragma once

#include <vector>

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#include "core.h"

//#include "layerstack.h"
//#include "event.h"
//#include "GUIManager.h"
//#include "camera.h"

namespace SKEngio {

    class Renderer {
        public:

            Renderer(WindowManager* winMan);

            virtual ~Renderer();

            void Render();

            bool InitGL();

            void InitGUI();

            void Draw();

            void OnEvent(Event* e);

            LayerStack* GetLayerStack();

            void addCamera(Camera* newCamera);
            void setActiveCamera(unsigned int camID);

        private:

            void HandleResize(int width, int height);
            void setPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);

            //just a reference (shared with Application)
            WindowManager* winMan;

            //the gui manager
            GUIManager* guiMan;

        	glm::mat4x4 mProjMatrix, mModelViewMatrix;

            LayerStack* layerStack;

            //float fovAngle = 45.0f;
            //float nearPlane = 0.01f;
            //float farPlane = 100.0f;

            std::vector<Camera*> cameraList;
            Camera* activeCamera;
    };
}
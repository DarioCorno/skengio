#pragma once

#ifndef SK_LIGHT_
#define SK_LIGHT_

#include "glm/glm.hpp"
#include "skengio/camera.h"
#include "skengio/entities/entity.h"
#include "skengio/utils/texture.h"
#include "skengio/utils/shaderProgram.h"
#include "skengio/utils/textureManager.h"

#include <memory>

namespace SKEngio  {

    class Light final : public Entity {
        public:

            glm::vec3 lightAmbientColor{ 0.5f, 0.5f, 0.5f };
            glm::vec3 lightDiffuseColor{ 0.8f, 0.8f, 0.8f };
            glm::vec3 lightSpecularColor{ 0.8f, 0.8f, 0.8f };

            void OnDetach();

            void SetPosition(float x, float y, float z);
            void SetDiffuse(float r, float g, float b);
            void SetSpecular(float r, float g, float b);
            void SetAmbient(float r, float g, float b);

            glm::vec3 GetPosition();
            glm::vec3 GetDiffuse();
            glm::vec3 GetAmbient();
            glm::vec3 GetSpecular();

            glm::mat4 getLightViewProjMatrix();

            void GenerateShadowMapBuffer(const unsigned int shadowMapFBO, const unsigned int shadowMapWidth, const unsigned int shadowMapHeight);
            void BeginShadowMapRender();
            void EndShadowMapRender();
            Texture* GetShadowTexture();

            bool enabled = true;

        private:

            bool hasShadowMapBuffer = false;
            unsigned int shadowMapWidth = 1024;
            unsigned int shadowMapHeight = 1024;
            unsigned int ShadowMap_FBOID{};
            unsigned int ShadowMap_RBO{};
            Texture* ShadowMap_Texture;

    };



}

#endif
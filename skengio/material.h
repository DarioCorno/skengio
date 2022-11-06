#pragma once

#ifndef SK_MATERIAL_
#define SK_MATERIAL_

#include "glm/glm.hpp"

#include "utils/texture.h"
#include "utils/shaderProgram.h"

namespace SKEngio  {
    class Material {
    public:

        int materialID = -1;

        Material();

        ~Material();

        void OnDestroy();

        glm::vec3 materialAmbientColor{ 0.4f, 0.4f, 0.4f };
        glm::vec3 materialDiffuseColor{ 0.6f, 0.6f, 0.6f };
        glm::vec3 materialSpecularColor{ 1.0f, 1.0f, 1.0f };
        float materialShininess = 32.0f;
        float materialReflectivity = 0.25f;

        Texture* diffuseTexture = nullptr;
        Texture* specularTexture = nullptr;
        Texture* normalTexture = nullptr;
        Texture* cubemapTexture = nullptr;

        void bind();
        void unbind();
        ShaderProgram* GetShader();
        void SetShader(ShaderProgram* _shader);

        void SetCubemap(Texture* _cubemap);

        void SetAmbient(float r, float g, float b);
        void SetDiffuse(float r, float g, float b);
        void SetSpecular(float r, float g, float b);
        void SetShininess(float _shiny);
        void SetReflectivity(float _refl);

        const glm::vec3& GetAmbient() const;
        const glm::vec3& GetDiffuse() const;
        const glm::vec3& GetSpecular() const;
        float GetShininess() const;
        float GetReflectivity() const;

        void LoadShader(const std::string& strPath, const std::string& strFileName, ShaderProgram::SHADERTYPE typeShader);
        void LoadShader(const std::string& strPath, const std::string& strFileName, ShaderProgram::SHADERTYPE typeShader, std::list<ShaderDefine> defines);
        void CreateProgram();

    private:
        ShaderProgram* shader = nullptr;


    };



}

#endif
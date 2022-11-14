#include "material.h"

#include "skengio/utils/textureManager.h"

namespace SKEngio {

    Material::Material() {
        shader = new ShaderProgram();
    }

    Material::~Material() {
    }

    void Material::OnDestroy() {
        shader->OnDestroy();
        delete shader;

        TextureManager tm = TextureManager::get();

        tm.DestroyTexture(diffuseTexture);

        //if (diffuseTexture != nullptr)
        //    delete diffuseTexture;

        tm.DestroyTexture(specularTexture);

        //if (specularTexture != nullptr)
        //    delete specularTexture;

        tm.DestroyTexture(cubemapTexture);

        //if (cubemapTexture != nullptr)
        //    delete cubemapTexture;

    }

    void Material::bind() {
        shader->bind();
        shader->SetMaterialUniforms(materialDiffuseColor, materialAmbientColor, materialSpecularColor, materialShininess, materialReflectivity);

        if(diffuseTexture != nullptr)
            shader->SetDiffTexture(diffuseTexture->textureUnit);

        shader->EnableDiffuseTexture(useDiffuseTexture);

        if (specularTexture != nullptr)
            shader->SetSpecularTexture(specularTexture->textureUnit);

        shader->EnableSpecularTexture(useSpecularTexture);

        if (cubemapTexture != nullptr)
            shader->SetCubeTexture(cubemapTexture->textureUnit);

        shader->EnableCubemapTexture(useCubemapTexture);

    }

    void Material::unbind() {
        shader->unbind();
    }

    ShaderProgram* Material::GetShader() {
        return shader;
    }

    void Material::SetShader(ShaderProgram* _shader) {
        if (shader == _shader)
            return;

        if (shader != nullptr) {
            shader->OnDestroy();
            delete shader;
        }

        shader = _shader;

    }

    void Material::LoadShader(const std::string& strPath, const std::string& strFileName, ShaderProgram::SHADERTYPE typeShader) {
        shader->LoadShader(strPath, strFileName, typeShader);
    }

    void Material::LoadShader(const std::string& strPath, const std::string& strFileName, ShaderProgram::SHADERTYPE typeShader, std::list<ShaderDefine> defines) {
        shader->LoadShader(strPath, strFileName, typeShader, defines);
    }

    void Material::CreateProgram() {
        shader->CreateProgram();
    }

    void Material::SetAmbient(float r, float g, float b) {
        materialAmbientColor = glm::vec3( r, g, b);
    }
    
    void Material::SetDiffuse(float r, float g, float b) {
        materialDiffuseColor = glm::vec3( r, g, b);
    }

    void Material::SetSpecular(float r, float g, float b) {
        materialSpecularColor = glm::vec3( r, g, b);
    }

    void Material::SetShininess(float shiny) {
        materialShininess = shiny;
    }

    const glm::vec3& Material::GetAmbient() const {
        return materialAmbientColor;
    }

    void Material::SetReflectivity(float _refl) {
        materialReflectivity = _refl;
    }

    const glm::vec3& Material::GetDiffuse() const {
        return materialDiffuseColor;
    }

    const glm::vec3& Material::GetSpecular() const {
        return materialSpecularColor;
    }

    float Material::GetShininess() const {
        return materialShininess;
    }

    float Material::GetReflectivity() const {
        return materialReflectivity;
    }

    void Material::SetCubemap(Texture* _cubemap) {
        cubemapTexture = _cubemap;
    }

}
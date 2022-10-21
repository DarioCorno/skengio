#include "light.h"

#include "skengio/utils/object.h"

namespace SKEngio {
    void Light::SetPosition(float x, float y, float z) {
        lightPosition = glm::vec3( x, y, z ) ;
        if (hasDebug) {
            lightdeb->resetTransforms();
            lightdeb->translate(x, y, z);
        }

    }

    void Light::SetDiffuse(float r, float g, float b ) {
        lightDiffuseColor = glm::vec3( r, g, b );
    }

    glm::vec3 Light::GetPosition() {
        return lightPosition;
    }

    glm::vec3 Light::GetDiffuse() {
        return lightDiffuseColor;
    }

    void Light::enableDebug() {
        hasDebug = true;

        lightdeb = std::make_unique<Object>();
        lightdeb->mesh = new Box();
        ((SKEngio::Box*)lightdeb->mesh)->Generate(0.4f, 0.4f, 0.4f, 1, 1, 1);
        lightdeb->mesh->buildInterleavedArray();
        lightdeb->mesh->createGLBuffers();

        lightdeb->shader = new SKEngio::ShaderProgram();
        lightdeb->shader->LoadShader("./shaders/", "utility.vert", SKEngio::ShaderProgram::VERTEX);
        lightdeb->shader->LoadShader("./shaders/", "utility.frag", SKEngio::ShaderProgram::FRAGMENT);
        lightdeb->shader->CreateProgram();
    }

    void Light::setDebugCamera(Camera* cam) {
        lightdeb->shader->SetCameraUniforms(cam);
    }

    void Light::drawDebug() {
        lightdeb->basicRender();
    }

}
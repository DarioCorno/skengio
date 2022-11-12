#include "light.h"

#include "skengio/entities/geometries/box.h"

namespace SKEngio {

    void Light::OnDetach() {
        Entity::OnDetach();
    }

    void Light::SetPosition(float x, float y, float z) {
        transform.setPosition( glm::vec3( x, y, z ) );
    }

    void Light::SetDiffuse(float r, float g, float b ) {
        lightDiffuseColor = glm::vec3( r, g, b );
    }

    void Light::SetSpecular(float r, float g, float b) {
        lightSpecularColor = glm::vec3(r, g, b);
    }

    void Light::SetAmbient(float r, float g, float b) {
        lightAmbientColor = glm::vec3(r, g, b);
    }

    glm::vec3 Light::GetPosition() {
        return transform.getGlobalPosition();
    }

    glm::vec3 Light::GetDiffuse() {
        return lightDiffuseColor;
    }

    glm::vec3 Light::GetAmbient() {
        return lightAmbientColor;
    }

    glm::vec3 Light::GetSpecular() {
        return lightSpecularColor;
    }

    glm::mat4 Light::getDirLightViewProjMatrix() {
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        //this must be scene bounding box
        float near_plane = 0.1f, far_plane = 100.0f;
        //size of projection should be scene bounding box
        lightProjection = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, near_plane, far_plane);
        //correct
        lightView = glm::lookAt(transform.getGlobalPosition(), transform.getGlobalPosition() + transform.getForward(), transform.getUp() );
        //wrong
        //glm::vec3 testTarget = transform.getBackward();
        //glm::vec3 testUp = transform.getUp();
        //lightView = glm::lookAt(transform.getGlobalPosition(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        lightSpaceMatrix = lightProjection * lightView;
        return lightSpaceMatrix;
    }

    void Light::GenerateDirShadowMapBuffer(const unsigned int shadowMapFBO, const unsigned int sMapWidth, const unsigned int sMapHeight) {

        shadowMapWidth = sMapWidth;
        shadowMapHeight = sMapHeight;

        ShadowMap_Texture = TextureManager::get().CreateShadowMapTexture(shadowMapWidth, shadowMapHeight);
        //bind the renderer buffer used for shadow depth
        ShadowMap_FBOID = shadowMapFBO;
        glBindFramebuffer(GL_FRAMEBUFFER, ShadowMap_FBOID);
        
        //attach this texture to the current (binded) frame buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ShadowMap_Texture->textureID, 0);

        //disable writes to color buffer
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            SK_LOG_ERR("ERROR Creating Shadow Render Buffer Object");
        }

        //glBindFramebuffer(GL_FRAMEBUFFER, 0);

        hasShadowMapBuffer = true;

    }

    void Light::BeginShadowMapRender() {
        //bind the frame buffer
        //glBindFramebuffer(GL_FRAMEBUFFER, ShadowMap_FBOID);

        ShadowMap_Texture->bind();

        //attach this light texture to the shadowmapFBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ShadowMap_Texture->textureID, 0);

        // render scene from light's point of view using the shadowMap shader
        glViewport(0, 0, shadowMapWidth, shadowMapHeight);
        glClearDepth(1.0);
        glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
    }

    Texture* Light::GetShadowTexture() {
        return ShadowMap_Texture;
    }

    void Light::EndShadowMapRender() {
        ShadowMap_Texture->unbind();
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


}
#include "texture.h"

#include "skengio/logger.h"
#include "skengio/utils/texturemanager.h"

#define GLEW_STATIC

namespace SKEngio {

    Texture::~Texture() {
        TextureManager::get().FreeData(data);
    }

    void Texture::bind() {
        
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        if (!isCubemap) {
            glBindTexture(GL_TEXTURE_2D, textureID);
        }
        else {
            glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        }
    }

    void Texture::unbind() {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        if (!isCubemap) {
            glDisable(GL_TEXTURE_2D);
        }
        else {
            glDisable(GL_TEXTURE_CUBE_MAP);
        }

    }

}

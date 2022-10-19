#include "textureManager.h"
#include "skengio/logger.h"

#ifndef STB_INCLUDED

#define STB_INCLUDED

#define STBI_NO_BMP
#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#endif


namespace SKEngio {

    /* Null, because instance will be initialized on demand. */
    TextureManager* TextureManager::instance = 0;

    unsigned int TextureManager::textureCount = 0;

    TextureManager* TextureManager::getInstance() {
        if (instance == 0)
            instance = new TextureManager();

        return instance;
    }

    TextureManager::TextureManager(){
        textureCount = 0;
    }

    SKEngio::Texture* TextureManager::Load(std::string fName, bool freeData) {
        Texture* texture = new Texture();

        texture->data = stbi_load(fName.c_str(), &texture->width, &texture->height, &texture->numChannels, 0);
        if (texture->data == NULL) {
            SK_LOG_ERR("TEXTURE ERROR! Cannot load " << fName.c_str());
            return NULL;
        }

        texture->textureUnit = textureCount++;

        glGenTextures(1, &texture->textureID);
        glActiveTexture(GL_TEXTURE0 + texture->textureUnit);
        glBindTexture(GL_TEXTURE_2D, texture->textureID);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);
        glGenerateMipmap(GL_TEXTURE_2D);

        if (freeData) {
            FreeData(texture->data);
            texture->data = NULL;
        }

        texture->loaded = true;

        return texture;
    }

    SKEngio::Texture* TextureManager::LoadCubemap(std::vector<std::string> facesFiles) {
        Texture* texture = new Texture();
        texture->textureUnit = textureCount++;

        glGenTextures(1, &texture->textureID);
        glActiveTexture(GL_TEXTURE0 + texture->textureUnit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture->textureID);

        int width, height, nrComponents;
        for (unsigned int i = 0; i < facesFiles.size(); i++)
        {
            unsigned char* data = stbi_load(facesFiles[i].c_str(), &width, &height, &nrComponents, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                SK_LOG("Cubemap texture failed to load at path: " << facesFiles[i]);
                stbi_image_free(data);
                return NULL;
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        texture->isCubemap = true;
        texture->data = NULL;
        texture->loaded = true;

        return texture;
    }
    SKEngio::Texture* TextureManager::CreateFrameBufferTexture(unsigned int width, unsigned int height) {
        Texture* texture = new Texture();
        texture->textureUnit = textureCount++;

        texture->width = width;
        texture->height = height;
        texture->isCubemap = false;
        texture->data = NULL;

        //create the texture for framebuffer
        glGenTextures(1, &texture->textureID);
        glActiveTexture(GL_TEXTURE0 + texture->textureUnit);
        glBindTexture(GL_TEXTURE_2D, texture->textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        texture->loaded = true;
        return texture;

    }

    SKEngio::Texture* TextureManager::CreateShadowMapTexture(unsigned int width, unsigned int height) {
        Texture* texture = new Texture();
        texture->textureUnit = textureCount++;

        texture->width = width;
        texture->height = height;
        texture->isCubemap = false;
        texture->data = NULL;

        glGenTextures(1, &texture->textureID);
        glActiveTexture(GL_TEXTURE0 + texture->textureUnit);
        glBindTexture(GL_TEXTURE_2D, texture->textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        texture->loaded = true;
        return texture;
    }

    void TextureManager::FreeData(unsigned char* data) {
        if (data) {
            stbi_image_free(data);
            data = NULL;
        }
    }

}
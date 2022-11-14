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

    void TextureManager::OnDestroy() {
        for (TextureSlot* ts : textureSlots) {
            delete ts;
        }

        textureSlots.clear();
    }

    Texture* TextureManager::Load(const std::string& fName, bool freeData) {

        int exists = AlreadyExists(fName);
        if (exists > -1) {
            textureSlots[exists]->useCount++;
            return textureSlots[exists]->texture;
        }
        else {
            TextureSlot* tSlot = new TextureSlot();
            tSlot->id = textureSlots.size();
            tSlot->fileName = fName;
            tSlot->texture = new Texture();

            unsigned char* data = stbi_load(fName.c_str(), &tSlot->texture->width, &tSlot->texture->height, &tSlot->texture->numChannels, 0);
            if (data == nullptr) {
                SK_LOG_ERR("TEXTURE ERROR! Cannot load " << fName.c_str());
                return nullptr;
            }

            tSlot->texture->textureUnit = textureSlots.size();  //textureCount++;
            textureSlots.push_back(tSlot);

            glGenTextures(1, &tSlot->texture->textureID);
            glActiveTexture(GL_TEXTURE0 + tSlot->texture->textureUnit);
            glBindTexture(GL_TEXTURE_2D, tSlot->texture->textureID);

            // set the texture wrapping/filtering options (on the currently bound texture object)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tSlot->texture->width, tSlot->texture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free(data);

            tSlot->texture->loaded = true;

            return tSlot->texture;

        }
    }

    Texture* TextureManager::LoadCubemap(const std::vector<std::string>& facesFiles) {
        int exists = AlreadyExists(facesFiles[0]);
        if (exists > -1) {
            textureSlots[exists]->useCount++;
            return textureSlots[exists]->texture;
        }
        else {
            TextureSlot* tSlot = new TextureSlot();
            tSlot->id = textureSlots.size();
            tSlot->fileName = facesFiles[0];
            tSlot->texture = new Texture();

            tSlot->texture->textureUnit = textureSlots.size(); //textureCount++;
            textureSlots.push_back(tSlot);

            glGenTextures(1, &tSlot->texture->textureID);
            glActiveTexture(GL_TEXTURE0 + tSlot->texture->textureUnit);
            glBindTexture(GL_TEXTURE_CUBE_MAP, tSlot->texture->textureID);

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
                    return nullptr;
                }
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            tSlot->texture->isCubemap = true;
            tSlot->texture->loaded = true;

            return tSlot->texture;
        }
    }

    Texture* TextureManager::CreateFrameBufferTexture(unsigned int width, unsigned int height) {
        TextureSlot* tSlot = new TextureSlot();
        tSlot->id = textureSlots.size();
        tSlot->fileName = "FBOTex" + std::to_string(textureSlots.size());
        tSlot->texture = new Texture();
        tSlot->texture->textureUnit = textureSlots.size(); //textureCount++;
        textureSlots.push_back(tSlot);

        tSlot->texture->width = width;
        tSlot->texture->height = height;
        tSlot->texture->isCubemap = false;

        //create the texture for framebuffer
        glGenTextures(1, &tSlot->texture->textureID);
        glActiveTexture(GL_TEXTURE0 + tSlot->texture->textureUnit);
        glBindTexture(GL_TEXTURE_2D, tSlot->texture->textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        tSlot->texture->loaded = true;
        return tSlot->texture;

    }

    Texture* TextureManager::CreateShadowMapTexture(unsigned int width, unsigned int height) {
        TextureSlot* tSlot = new TextureSlot();
        tSlot->id = textureSlots.size();
        tSlot->fileName = "SHADOWMAPTex" + std::to_string(textureSlots.size());
        tSlot->texture = new Texture();
        tSlot->texture->textureUnit = textureSlots.size(); //textureCount++;
        textureSlots.push_back(tSlot);

        //Texture* texture =  new Texture();
        //texture->textureUnit = textureCount++;

        tSlot->texture->width = width;
        tSlot->texture->height = height;
        tSlot->texture->isCubemap = false;

        glGenTextures(1, &tSlot->texture->textureID);
        glActiveTexture(GL_TEXTURE0 + tSlot->texture->textureUnit);
        glBindTexture(GL_TEXTURE_2D, tSlot->texture->textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        tSlot->texture->loaded = true;
        return tSlot->texture;
    }

    Texture* TextureManager::CreateCubemapShadowMapTexture(unsigned int width, unsigned int height) {
        TextureSlot* tSlot = new TextureSlot();
        tSlot->id = textureSlots.size();
        tSlot->fileName = "SHADOWCUBEMAPTex" + std::to_string(textureSlots.size());
        tSlot->texture = new Texture();
        tSlot->texture->textureUnit = textureSlots.size(); //textureCount++;
        textureSlots.push_back(tSlot);

        //Texture* texture = new Texture();
        //texture->textureUnit = textureCount++;

        tSlot->texture->width = width;
        tSlot->texture->height = height;
        tSlot->texture->isCubemap = true;

        glGenTextures(1, &tSlot->texture->textureID);
        glActiveTexture(GL_TEXTURE0 + tSlot->texture->textureUnit);
        glBindTexture(GL_TEXTURE_2D, tSlot->texture->textureID);

        for (unsigned int i = 0; i < 6; ++i)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        tSlot->texture->loaded = true;
        return tSlot->texture;
    }

    bool TextureManager::DestroyTexture(Texture* texture) {
        bool deleted = false;
        if (texture != nullptr) {
            int texIdx = texture->textureUnit;
            TextureSlot* ts = textureSlots[texIdx];
            ts->useCount--;
            if (ts->useCount == 0) {
                delete texture;
                texture = nullptr;
                deleted = true;
            }
        }

        return deleted;
    }

    int TextureManager::AlreadyExists(std::string fileName) {
        int res = -1;
        for (int idx = 0; idx < textureSlots.size(); idx++) {
            if (textureSlots[idx]->fileName == fileName) {
                res = idx;
                break;
            }
        }

        return res;
    }

}
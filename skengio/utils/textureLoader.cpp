#include "textureLoader.h"

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

    TextureLoader::TextureLoader() {
        textureID = 0;
        data = NULL;
        loaded = false;
        isCubemap = false;
    }


    TextureLoader::~TextureLoader() {
        if(data != NULL)
            stbi_image_free(data);
    }

    bool TextureLoader::Load(std::string strFileName, bool freeData) {

        data = stbi_load(strFileName.c_str(), &width, &height, &numChannels, 0);
        if(data == NULL) {
            SK_LOG_ERR("TEXTURE ERROR! Cannot load " << strFileName.c_str() );
            return false;
        }

        glGenTextures(1, &textureID);
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, textureID);  

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);            

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);            

        if(freeData) {
            stbi_image_free(data);
            data = NULL;
        }

        return true;
    }

    bool TextureLoader::LoadCubemap(vector<std::string> facesFiles, bool freeData) {
        glGenTextures(1, &textureID);
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

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
                return false;
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        isCubemap = true;

        return true;
    }

    void TextureLoader::bind() {
        
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        if (!isCubemap) {
            glBindTexture(GL_TEXTURE_2D, textureID);
        }
        else {
            glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        }
    }

    void TextureLoader::unbind() {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        if (!isCubemap) {
            glDisable(GL_TEXTURE_2D);
        }
        else {
            glDisable(GL_TEXTURE_CUBE_MAP);
        }

    }

}

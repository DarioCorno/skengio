#pragma once

#ifndef _SK_TEXTUREMANAGER_
#define _SK_TEXTUREMANAGER_

#include <GLEW/glew.h>
#include <iostream>
#include <string>
#include "texture.h"

namespace SKEngio {

    class TextureManager {

	    private:
		    static TextureManager* instance;

            /* Private constructor to prevent instancing. */
            TextureManager();

            // prevent copying object
            TextureManager(const TextureManager&) = delete;
            TextureManager(TextureManager&&) = delete;
            TextureManager& operator=(const TextureManager&) = delete;
            TextureManager& operator=(TextureManager&&) = delete;

            static unsigned int textureCount;

        public:

            static TextureManager* getInstance();

            static SKEngio::Texture* Load(std::string fName, bool freeData);
            static SKEngio::Texture* LoadCubemap(std::vector<std::string> facesFiles);
            static SKEngio::Texture* CreateShadowMapTexture(unsigned int width, unsigned int height);
            static SKEngio::Texture* CreateFrameBufferTexture(unsigned int width, unsigned int height);

            static void FreeData(unsigned char* data);

	};
}

#endif
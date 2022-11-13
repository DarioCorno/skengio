#pragma once

#ifndef SK_TEXTUREMANAGER_
#define SK_TEXTUREMANAGER_

#include <string>
#include <vector>

#include "texture.h"
#include "singleton.h"

namespace SKEngio {

    class TextureManager final : public Singleton<TextureManager>{

	    private:
            /* Private constructor to prevent instancing. */
            //TextureManager() = default;

            unsigned int textureCount = 0;

        public:

            // prevent copying object
            //TextureManager(const TextureManager&) = delete;
            //TextureManager(TextureManager&&) = delete;
            //TextureManager& operator=(const TextureManager&) = delete;
            //TextureManager& operator=(TextureManager&&) = delete;

            //static TextureManager& getInstance();

            Texture* Load(const std::string& fName, bool freeData);
            Texture* LoadCubemap(const std::vector<std::string>& facesFiles);
            Texture* CreateShadowMapTexture(unsigned int width, unsigned int height);
            Texture* CreateFrameBufferTexture(unsigned int width, unsigned int height);
            Texture* CreateCubemapShadowMapTexture(unsigned int width, unsigned int height);
            void FreeData(unsigned char* data);

	};
}

#endif
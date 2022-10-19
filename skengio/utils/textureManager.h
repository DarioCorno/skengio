#pragma once

#ifndef SK_TEXTUREMANAGER_
#define SK_TEXTUREMANAGER_

#include <string>
#include <vector>

#include "texture.h"

namespace SKEngio {

    class TextureManager {

	    private:
            /* Private constructor to prevent instancing. */
            TextureManager() = default;

            unsigned int textureCount = 0;

        public:

            // prevent copying object
            TextureManager(const TextureManager&) = delete;
            TextureManager(TextureManager&&) = delete;
            TextureManager& operator=(const TextureManager&) = delete;
            TextureManager& operator=(TextureManager&&) = delete;

            static TextureManager& getInstance();

            std::unique_ptr<Texture> Load(const std::string& fName, bool freeData);
            std::unique_ptr<Texture> LoadCubemap(const std::vector<std::string>& facesFiles);
            std::unique_ptr<Texture> CreateShadowMapTexture(unsigned int width, unsigned int height);
            std::unique_ptr<Texture> CreateFrameBufferTexture(unsigned int width, unsigned int height);

            void FreeData(unsigned char* data);

	};
}

#endif
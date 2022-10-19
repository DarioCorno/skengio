#pragma once

#ifndef _SK_TEXTUREMANAGER_
#define _SK_TEXTUREMANAGER_

#include <iostream>
#include <string>
#include "textureLoader.h"

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

            static SKEngio::TextureLoader* Load(std::string fName, bool freeData);
            static SKEngio::TextureLoader* LoadCubemap(vector<std::string> facesFiles, bool freeData);
             

	};
}

#endif
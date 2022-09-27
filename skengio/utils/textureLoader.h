#pragma once

#include "../core.h"

#include <string>
#include <iostream>

namespace SKEngio {

    class TextureLoader {
        public:

        int width, height;
        int numChannels;
        unsigned int textureID;
        unsigned char* data = NULL;
        bool loaded = false;

        TextureLoader();

        ~TextureLoader();

        bool Load(std::string strFileName, bool freeData);

        void bind();

    };
}

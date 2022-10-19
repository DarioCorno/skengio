#pragma once

#ifndef _SK_TEXTURE_
#define _SK_TEXTURE_

#include <string>
#include <iostream>
#include <vector>

namespace SKEngio {

    class TextureLoader {
        public:

            int width;
            int height;
            int numChannels;
            unsigned int textureID;
            unsigned int textureUnit;
            unsigned char* data = NULL;
            bool loaded = false;
            bool isCubemap = false;

            TextureLoader();

            ~TextureLoader();

            bool Load(std::string strFileName, bool freeData);

            bool LoadCubemap(std::vector<std::string> facesFiles, bool freeData);

            void bind();

            void unbind();

        private:

    };
}


#endif
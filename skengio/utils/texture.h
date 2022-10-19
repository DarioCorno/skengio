#pragma once

#ifndef _SK_TEXTURE_
#define _SK_TEXTURE_

#include <GLEW/glew.h>

#include <string>
#include <iostream>
#include <vector>

namespace SKEngio {

    class Texture {
        public:

            int width;
            int height;
            int numChannels;
            unsigned int textureID;
            unsigned int textureUnit;
            unsigned char* data = NULL;
            bool loaded = false;
            bool isCubemap = false;

            Texture();

            ~Texture();

            void bind();

            void unbind();

        private:

    };
}


#endif
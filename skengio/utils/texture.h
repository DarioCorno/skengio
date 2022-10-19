#pragma once

#ifndef SK_TEXTURE_
#define SK_TEXTURE_

#include <GLEW/glew.h>

#include <iostream>

namespace SKEngio {

    class Texture {
        public:

            int width;
            int height;
            int numChannels;
            unsigned int textureID = 0;
            unsigned int textureUnit;
            unsigned char* data = nullptr;
            bool loaded = false;
            bool isCubemap = false;

            Texture() = default;

            // prevent copying object
            Texture(const Texture&) = delete;
            Texture(Texture&&) = delete;
            Texture& operator=(const Texture&) = delete;
            Texture& operator=(Texture&&) = delete;

            ~Texture();

            void bind();

            void unbind();

        private:

    };
}


#endif
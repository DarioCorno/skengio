#include "textureManager.h"

namespace SKEngio {

    /* Null, because instance will be initialized on demand. */
    TextureManager* TextureManager::instance = 0;

    unsigned int TextureManager::textureCount = 0;

    TextureManager* TextureManager::getInstance() {
        if (instance == 0)
            instance = new TextureManager();

        return instance;
    }

    TextureManager::TextureManager(){
        textureCount = 0;
    }

    SKEngio::TextureLoader* TextureManager::Load(std::string fName, bool freeData) {
        TextureLoader* texture = new TextureLoader();
        texture->textureUnit = textureCount++;
        texture->Load(fName, freeData);

        return texture;
    }

    SKEngio::TextureLoader* TextureManager::LoadCubemap(std::vector<std::string> facesFiles, bool freeData) {
        TextureLoader* texture = new TextureLoader();
        texture->textureUnit = textureCount++;
        texture->LoadCubemap(facesFiles, freeData);

        return texture;
    }

}
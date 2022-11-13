//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include "skengio/application.h"
#include "skengio/renderer.h"
#include "skengio/logger.h"
#include "skengio/window.h"
#include "skengio/scene.h"
//#include "effects/effectLayerONE.h"
//#include "effects/effectLayerTWO.h"
#include "effects/sceneONE.h"

#include <iostream>
#include <sstream>

static void errorCB(int error, const char* description) {
    std::cout << "ERROR: " << description << std::endl;
}

int main()
{
    glm::quat yawpitchroll = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians( -45.0f) ));
    //glm::quat orientationQuat = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
    glm::quat orientationQuat = glm::angleAxis(0.0f, 0.0f, 0.0f, 0.0f);
    glm::mat4 model = glm::mat4_cast(glm::inverse(orientationQuat * yawpitchroll));

    SKEngio::WindowSpecs winSpecs;
    winSpecs.width = 1024;
    winSpecs.height = 768;
    winSpecs.caption = "SKEngio test";
    winSpecs.fullscreen = false;
    winSpecs.canResize = false;

    SKEngio::Application app; // = new SKEngio::Application(&winSpecs, &errorCB);
    app.Initialize(&winSpecs, &errorCB);

    SKEngio::SceneONE* scene = new SKEngio::SceneONE();
    scene->SetName("Scena 01");
    scene->SetCamera(45.0f, "cam01");
    SKEngio::Renderer::get().AddScene(scene);

    scene->music = new SKEngio::AudioSource();
    scene->music->loadStream("moby_elekfunk.mp3", SKEngio::AudioType::Stream);
    scene->music->setVolume(1000);

    //scene->music->play();
    app.Run();

    app.Destroy();
    delete scene;

    //_CrtDumpMemoryLeaks();

    return 0;
}
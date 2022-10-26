//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include "skengio/application.h"
#include "skengio/renderer.h"
#include "skengio/logger.h"
#include "skengio/window.h"
#include "skengio/scene.h"
#include "effects/effectLayerONE.h"
#include "effects/effectLayerTWO.h"

#include <iostream>
#include <sstream>

static void errorCB(int error, const char* description) {
    std::cout << "ERROR: " << description << std::endl;
}

int main()
{

    SKEngio::WindowSpecs winSpecs;
    winSpecs.width = 1024;
    winSpecs.height = 768;
    winSpecs.caption = "SKEngio test";
    winSpecs.fullscreen = false;

    SKEngio::Application app; // = new SKEngio::Application(&winSpecs, &errorCB);
    app.Initialize(&winSpecs, &errorCB);

    //SKEngio::Renderer* rend = app.GetRenderer();
    //if (!rend) {
    //    SK_LOG("Cannot retrieve renderer");
    //}
    //rend->NewCamera(45.0f, "cam01");

    SKEngio::Scene* scene = new SKEngio::Scene(1);
    scene->SetName("Scena 01");
    scene->SetCamera(45.0f, "cam01");
    //rend->AddScene(scene);
    SKEngio::Renderer::get().AddScene(scene);

    scene->music = new SKEngio::AudioSource();
    scene->music->loadStream("moby_elekfunk.mp3", SKEngio::AudioType::Stream);
    scene->music->setVolume(1000);

    //add a triangle layer  
    EffectOne* mylayerOne = new EffectOne();
    mylayerOne->SetId(0);
    scene->PushLayer(mylayerOne);

    //add a torus
    EffectTwo* mylayerTwo = new EffectTwo();
    mylayerTwo->SetId(1);
    scene->PushLayer(mylayerTwo);

    //scene->music->play();
    app.Run();

    app.Destroy();
    delete scene;
    delete mylayerOne;
    delete mylayerTwo;

    //_CrtDumpMemoryLeaks();

    return 0;
}
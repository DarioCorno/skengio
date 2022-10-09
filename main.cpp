
#include "skengio/core.h"
#include "skengio/logger.h"
#include "effects/effectLayerONE.h"
#include "effects/effectLayerTWO.h"
//#include "effects/effectLayerTHREE.h"

#include <iostream>
#include <sstream>
#include <string>

static void errorCB(int error, const char* description) {
    std::cout << "ERROR: " << description << std::endl;
}

int main(void)
{

    SKEngio::WindowSpecs winSpecs;
    winSpecs.width = 1024;
    winSpecs.height = 768;
    winSpecs.caption = "SKEngio test";

    SKEngio::Application* app = new SKEngio::Application(&winSpecs, &errorCB);

    SKEngio::Renderer* rend = app->GetRenderer();
    if(!rend) {
        SK_LOG("Cannot retrieve renderer");
    }


    SKEngio::Scene* scene = new SKEngio::Scene(1);
    scene->SetName("Scena 01");
    rend->AddScene(scene);

    //camera1, willbe used by scenes -> layerONE
    unsigned int camID = 1;
    SKEngio::Camera* cam = new SKEngio::Camera(winSpecs.width, winSpecs.height, 45.0f, camID);
    scene->addCamera(cam);
    scene->setActiveCamera(camID);

    SKEngio::LayerStack* ls = scene->GetLayerStack();

    //add a triangle layer  (example of using ImGUI to set variables, shaderProgram class and renderer activeCamera)
    EffectOne* mylayerOne = new EffectOne();
    mylayerOne->SetId(0);
    ls->PushLayer( mylayerOne );

    //add another cool triangle (example of defining a shader by code + transform matrix)
    EffectTwo* mylayerTwo = new EffectTwo();
    mylayerTwo->SetId(1);
    ls->PushLayer( mylayerTwo );

    //add another cool triangle (example of loading a shader + texture loading)
    //EffectThree* mylayerThree = new EffectThree();
    //mylayerThree->SetId(1);
    //ls->PushLayer( mylayerThree );


    app->Run();

    delete app;
    return 0;
}
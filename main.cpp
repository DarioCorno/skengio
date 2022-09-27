
#include "skengio/core.h"
#include "effects/effectLayerONE.h"
#include "effects/effectLayerTWO.h"
#include "effects/effectLayerTHREE.h"

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
        std::cout << "Cannot retrieve renderer" << std::endl;
    }
    SKEngio::LayerStack* ls = rend->GetLayerStack();

    //add a triangle layer  (example of using ImGUI to set variables and shaderProgram class)
    EffectOne* mylayerOne = new EffectOne();
    mylayerOne->SetId(0);
    ls->PushLayer( mylayerOne );

    //add another cool triangle (example of defining a shader by code + transform matrix)
    EffectTwo* mylayerTwo = new EffectTwo();
    mylayerTwo->SetId(1);
    ls->PushLayer( mylayerTwo );

    //add another cool triangle (example of loading a shader + texture loading)
    EffectThree* mylayerThree = new EffectThree();
    mylayerThree->SetId(1);
    ls->PushLayer( mylayerThree );


    app->Run();

    delete app;
    return 0;
}

#include "skengio/core.h"
#include "effects/effectLayerONE.h"
#include "effects/effectLayerTWO.h"

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

    //add some triangle layers
    EffectOne* mylayerOne = new EffectOne();
    mylayerOne->SetId(0);
    ls->PushLayer( mylayerOne );

    EffectTwo* mylayerTwo = new EffectTwo();
    mylayerTwo->SetId(1);
    ls->PushLayer( mylayerTwo );

    app->Run();

    delete app;
    return 0;
}
#include "layer.h"

#include "defines.h"

namespace SKEngio {

    Layer::Layer() {

    }

    Layer::~Layer() {

    }

    void Layer::OnAttach() {
        //std::cout << "Layer attached" << this->GetId() << std::endl;
    }

    void Layer::OnDetach() {
        //std::cout << "Detaching Layer" << this->GetId() << std::endl;
    }

    void Layer::OnUpdate(SKEngio::RenderParams* rp) {
        
    }    

    void Layer::OnDrawGUI(SKEngio::RenderParams* rp) {

    }

    void Layer::OnDraw(SKEngio::RenderParams* rp) {

    }

    void Layer::OnEvent(Event* e) {
        
    }

    void Layer::SetId(unsigned int id) {
        this->id = id;
    }

    unsigned int Layer::GetId() {
        return id;
    }

    //void Layer::AddObject(Object* obj) {
    //    //objects.push_back(obj);
    //}

}

#include "layer.h"

namespace SKEngio {
    void Layer::OnAttach() {
        //std::cout << "Layer attached" << this->GetId() << std::endl;
    }

    void Layer::OnDetach() {
        //std::cout << "Detaching Layer" << this->GetId() << std::endl;
    }

    void Layer::OnUpdate(RenderParams* rp) {
        
    }    

    void Layer::OnDrawGUI(RenderParams* rp) {

    }

    void Layer::OnDraw(RenderParams* rp) {

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

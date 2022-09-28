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

    void Layer::OnUpdate(float t) {
        
    }    

    void Layer::OnDrawGUI(float t) {

    }

    void Layer::OnDraw(float t) {

    }

    void Layer::OnEvent(Event* e) {
        
    }

    void Layer::SetId(unsigned int id) {
        this->id = id;
    }

    unsigned int Layer::GetId() {
        return id;
    }

}

#include <skengio/event.h>

#include <stdlib.h>

namespace SKEngio {
    void Event::SetKeyPress(int _key, int _scancode, int _action, int _mods) {
        type = EventType::KeyPress;
        key = _key;
        scancode = _scancode;
        action = _action;
        mods = _mods;
    }

    void Event::SetKeyRelease(int _key, int _scancode, int _action, int _mods) {
        type = EventType::KeyRelease;
        key = _key;
        scancode = _scancode;
        action = _action;
        mods = _mods;
    }

    void Event::SetMousePress(int _button, int _action, int _mods) {
        type = EventType::MousePress;
        button = _button;
        action = _action;
        mods = _mods;
    }

    void Event::SetMouseRelease(int _button, int _action, int _mods) {
        type = EventType::MouseRelease;
        button = _button;
        action = _action;
        mods = _mods;
    }

    void Event::SetMouseMove(float _xpos, float _ypos) {
        type = EventType::MouseMove;
        xPos = _xpos;
        yPos = _ypos;
    }

    void Event::SetMouseScroll(float _xoffset, float _yoffset) {
        type = EventType::MouseScroll;
        xOffset = _xoffset;
        yOffset = _yoffset;
    }

    void Event::SetResize(int _width, int _height) {
        type = EventType::Resize;
        width = _width;
        height = _height;
    }


}
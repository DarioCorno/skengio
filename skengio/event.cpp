#include "event.h"

namespace SKEngio {
    Event::Event() {
    };

    Event::~Event() { };

    void Event::SetKeyPress(int _key, int _scancode, int _action, int _mods) {
        type = EVENT_TYPE_KEYPRESS;
        key = _key;
        scancode = _scancode;
        action = _action;
        mods = _mods;
    };
    void Event::SetKeyRelease(int _key, int _scancode, int _action, int _mods) {
        type = EVENT_TYPE_KEYRELEASE;
        key = _key;
        scancode = _scancode;
        action = _action;
        mods = _mods;
    };
    void Event::SetMousePress(int _button, int _action, int _mods) {
        type = EVENT_TYPE_MOUSEPRESS;
        button = _button;
        action = _action;
        mods = _mods;
    };
    void Event::SetMouseRelease(int _button, int _action, int _mods) {
        type = EVENT_TYPE_MOUSERELEASE;
        button = _button;
        action = _action;
        mods = _mods;
    };
    void Event::SetMouseMove(float _xpos, float _ypos) {
        type = EVENT_TYPE_MOUSEMOVE;
        xPos = _xpos;
        yPos = _ypos;
    };
    void Event::SetMouseScroll(float _xoffset, float _yoffset) {
        type = EVENT_TYPE_MOUSESCROLL;
        xOffset = _xoffset;
        yOffset = _yoffset;
    };
    void Event::SetResize(int _width, int _height) {
        type = EVENT_TYPE_RESIZE;
        width = _width;
        height = _height;
    };


}
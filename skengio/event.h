#pragma once

#ifndef _SK_EVENT_
#define _SK_EVENT_

#define EVENT_TYPE_KEYPRESS 0
#define EVENT_TYPE_KEYRELEASE 1
#define EVENT_TYPE_MOUSEMOVE 2
#define EVENT_TYPE_MOUSEPRESS 3
#define EVENT_TYPE_MOUSERELEASE 4
#define EVENT_TYPE_MOUSESCROLL 5
#define EVENT_TYPE_RESIZE 6

namespace SKEngio {

    class Event {
        public:

            Event();

            ~Event();

            void SetKeyPress(int _key, int _scancode, int _action, int _mods);
            void SetKeyRelease(int _key, int _scancode, int _action, int _mods);
            void SetMousePress(int _button, int _action, int _mods);
            void SetMouseRelease(int _button, int _action, int _mods);
            void SetMouseMove(float _xpos, float _ypos);
            void SetMouseScroll(float _xoffset, float _yoffset);
            void SetResize(int _width, int _height);

            int type;

            //resize
            int width = 0;
            int height = 0;

            //keyboard
            int key = 0;
            int scancode = 0;
            int action = 0;
            int mods = 0;

            //mouseclick
            int button = 0;

            //mousemove
            float xPos = 0;
            float yPos = 0;

            //mouse scroll
            float xOffset = 0;
            float yOffset = 0;
    };

}


#endif
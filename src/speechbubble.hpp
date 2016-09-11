//
//  speechbubble.hpp
//  digital
//
//  Created by Bryce on 9/4/16.
//
//

#ifndef speechbubble_hpp
#define speechbubble_hpp

#include <stdio.h>
#include "widget.hpp"

class SpeechBubble : public Widget {
protected:
    Widget *speaker;
    string text;
    int dir;
    
public:
    enum Direction {
        UP,
        DOWN,
    };
    
    SpeechBubble(Widget *speaker, string text="", bool direction=UP);
    void say(string msg);
    void draw() override;
};

#endif /* speechbubble_hpp */

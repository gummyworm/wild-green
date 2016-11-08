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
#include "entity.hpp"
#include "widget.hpp"

class SpeechBubble : public Widget {
protected:
    Widget *speakerPortrait;
    Entity *speaker;
    shared_ptr<Entity> hovered;
    
    string text;
    int dir;
    gl::TextureFontRef font;
    
    Timer fadeTimer;
    float fadeTime;
    
    Rectf rect;
public:
    enum Direction {
        UP,
        DOWN,
    };
    
    SpeechBubble(Widget *speakerPortrait, string text="", Entity *speaker=nullptr, bool direction=UP, float fadeTime=-1.0f);
    void say(string msg);
    bool onMouseDown(MouseEvent event) override;
    bool onMouseUp(MouseEvent event) override;
    bool onMouseDrag(MouseEvent event) override;
    void update() override;
    
    string getText() {return text;}
    Entity* getSpeaker() {return speaker;}
};

class SpeechBubble2 : public SpeechBubble {
public:
    SpeechBubble2(Widget *speakerPortrait, string text="", Entity* speaker=nullptr, bool direction=UP);
    void say(string msg);
    void draw() override;
};

class SpeechBubble3 : public SpeechBubble {
protected:
public:
    SpeechBubble3(Entity *speaker=nullptr, string text="", bool direction=UP);
    void say(string msg);
    
    void draw() override;
    void update() override;
};

#endif /* speechbubble_hpp */

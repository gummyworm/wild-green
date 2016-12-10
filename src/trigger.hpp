//
//  trigger.hpp
//  digital
//
//  Created by Bryce on 11/17/16.
//
//

#ifndef trigger_hpp
#define trigger_hpp

#include <stdio.h>
#include <functional>
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "entity.hpp"
#include "speechbubble.hpp"
#include "game.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Trigger {
protected:
    function<void()> activate;
    bool rearm;
    bool triggered;
public:
    Trigger(function<void()> activate=nullptr): activate(activate), rearm(false), triggered(false) {}
    virtual bool test(Entity *e) {return false;}
    virtual void draw() {}
};

class PositionTrigger : public Trigger {
protected:
    vec3 pos;
    float radius;
public:
    PositionTrigger(vec3 pos, float radius, function<void()> activate)
    :Trigger(activate), pos(pos), radius(radius) {}
    
    bool test(Entity *e) override;
    void draw() override;
};

class TextTrigger : public PositionTrigger {
    string text;
public:
    TextTrigger(vec3 pos, float radius, string text)
    :PositionTrigger(pos, radius, nullptr),
    text(text) {
        activate = [this](){game::speechMgr.say(nullptr, this->text);};
    }
};

#endif /* trigger_hpp */

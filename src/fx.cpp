//
//  fx.cpp
//  digital
//
//  Created by Bryce on 12/1/16.
//
//

#include "fx.hpp"
#include "game.h"

struct Effect{
    Timer t;
    float duration;
    int repeats;
    function<void()> onComplete;
    function<void()> update;
    Effect(float duration, function<void()> update, function<void()> onComplete): duration(duration), repeats(0), update(update), onComplete(onComplete) {
        t.start();
    }
};

static vector<Effect*> effects;

struct Flash {
    ColorA c1, c2;
    bool toggle;
    void toggleColor() {
        if(toggle)
            game::setClearColor(c1);
        else
            game::setClearColor(c2);
        toggle = !toggle;
    }
    Flash(ColorA c1, ColorA c2)
    :c1(c1),
    c2(c2),
    toggle(true) {
    }
};

void fx::flash(ColorA color1, ColorA color2, float duration, int repeats)
{
    game::setClearColor(color2);
    
    auto f = new Flash(color1, color2);
    
    function<void()> done = [f](){f->toggleColor();};
    auto e = new Effect(duration, nullptr, done);
    e->repeats = repeats;
    effects.push_back(e);
}

void fx::update()
{
    vector<Effect*> updated;
    for(auto e : effects) {
        if(e->t.getSeconds() <= e->duration) {
            if(e->update != nullptr)
                e->update();
            updated.push_back(e);
        } else {
            e->onComplete();
            e->repeats--;
            if(e->repeats < 0) {
                delete e;
            } else {
                e->t.start();
                updated.push_back(e);
            }
        }
    }
    effects = updated;
}
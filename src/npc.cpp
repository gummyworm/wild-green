//
//  npc.cpp
//  digital
//
//  Created by Bryce on 10/2/16.
//
//

#include "npc.hpp"


void NPC::onSpeak(Entity *other, string msg)
{
    if(other == nullptr)
        return;
}

void NPC::say(string msg)
{
    speechBubbles.push_back(unique_ptr<SpeechBubble>(new SpeechBubble3(this, msg)));
}

void NPC::label(Camera cam)
{
    for(auto& s : speechBubbles) {
        s->draw();
        s->apply();
    }
}

void NPC::update()
{
    Entity::update();
    if(ai != nullptr)
        ai->update();
    for(auto& s : speechBubbles)
        s->update();
}
//
//  speech.cpp
//  digital
//
//  Created by Bryce on 10/6/16.
//
//

#include "speech.hpp"

void SpeechManager::say(Entity *speaker, string msg)
{
    newSpeeches.push_back(unique_ptr<SpeechBubble>(new SpeechBubble3(speaker, msg)));
}

void SpeechManager::say(unique_ptr<SpeechBubble> speech)
{
    newSpeeches.push_back(move(speech));
}

void SpeechManager::update()
{
    for(auto&& s : newSpeeches)
        speeches.push_back(move(s));
    newSpeeches.clear();
    
    for(auto&& s : speeches) {
        s->update();
    }
}

void SpeechManager::draw()
{
    for(auto&& s : speeches) {
        s->draw();
        s->apply();
    }
}

bool SpeechManager::onMouseDown(MouseEvent event)
{
    for(auto&& s : speeches) {
        s->onMouseDown(event);
    }
    return false;
}

bool SpeechManager::onMouseUp(MouseEvent event)
{
    for(auto&& s : speeches) {
        s->onMouseUp(event);
    }
    return false;
}

bool SpeechManager::onMouseDrag(MouseEvent event)
{
    for(auto&& s : speeches) {
        s->onMouseDrag(event);
    }
    return false;
}
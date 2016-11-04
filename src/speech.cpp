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
    
    for(auto&& s : speeches) {
        s->update();
    }
}

void SpeechManager::draw()
{
    for(auto&& s : speeches) {
        s->draw();
    }
}
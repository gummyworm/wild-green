//
//  speech.cpp
//  digital
//
//  Created by Bryce on 10/6/16.
//
//

#include <iterator>
#include "speech.hpp"

void SpeechManager::say(Entity *speaker, string msg)
{
    if(speaker != nullptr)
        newSpeeches.push_back(unique_ptr<SpeechBubble>(new SpeechBubble3(speaker, msg)));
    else
        newSpeeches.push_back(unique_ptr<SpeechBubble>(new SpeechBubble2(nullptr, msg)));
}

void SpeechManager::say(unique_ptr<SpeechBubble> speech)
{
    newSpeeches.push_back(move(speech));
}

void SpeechManager::silence(Entity *speaker)
{
    return;
//    vector<unique_ptr<SpeechBubble>> updated;
//    updated.reserve(speeches.size());
//    
//    for(auto&& s : speeches) {
//        if(s->getSpeaker() != speaker) {
//            updated.push_back(move(s));
//            newSpeeches.push_back(move(s));
//        }
//    }
//    speeches.clear();
//    update();
}

void SpeechManager::update()
{
    move(newSpeeches.begin(), newSpeeches.end(), back_inserter(speeches));
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
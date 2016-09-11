//
//  conversation.cpp
//  digital
//
//  Created by Bryce on 9/4/16.
//
//

#include "conversation.hpp"


Conversation::Conversation()
{
}

void Conversation::say(shared_ptr<SpeechBubble> speaker, string msg)
{
    auto s = find(speakers.begin(), speakers.end(), speaker);
    (*s)->say(msg);
}

void Conversation::addSpeaker(shared_ptr<SpeechBubble> speaker)
{
    speakers.push_back(speaker);
}
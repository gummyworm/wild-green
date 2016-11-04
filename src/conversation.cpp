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

void Conversation::say(shared_ptr<SpeechBubble> speaker, shared_ptr<SpeechBubble> listener, string msg)
{
    auto s = find(speakers.begin(), speakers.end(), speaker);
    s->get()->say(msg);
}

void Conversation::say(shared_ptr<SpeechBubble> speaker, string msg)
{
    say(speaker, nullptr, msg);
}

void Conversation::addSpeaker(shared_ptr<SpeechBubble> speaker)
{
    speakers.push_back(speaker);
}
//
//  conversation.hpp
//  digital
//
//  Created by Bryce on 9/4/16.
//
//

#ifndef conversation_hpp
#define conversation_hpp

#include <algorithm>
#include <memory>
#include <stdio.h>
#include <vector>
#include "speechbubble.hpp"


using namespace std;

class Conversation {
    vector<shared_ptr<SpeechBubble>> speakers;
public:
    Conversation();
    void addSpeaker(shared_ptr<SpeechBubble> speaker);
    void say(shared_ptr<SpeechBubble> speaker, shared_ptr<SpeechBubble> listener, string msg);
    void say(shared_ptr<SpeechBubble> speaker, string msg);
    
    vector<shared_ptr<SpeechBubble>> getSpeakers() {return speakers;}
};

#endif /* conversation_hpp */

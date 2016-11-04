//
//  npc.hpp
//  digital
//
//  Created by Bryce on 10/2/16.
//
//

#ifndef npc_hpp
#define npc_hpp

#include <stdio.h>
#include "ai_pattern.hpp"
#include "entity.hpp"
#include "speechbubble.hpp"

class NPC : public Entity {
protected:
    class Response : public SpeechBubble {
        string text;
        function<void(string)> onRespond;
    public:
        Response(string text="", function<void(string)> onRespond=nullptr):SpeechBubble(this, text),text(text), onRespond(onRespond) {}
        void respond() {
            if(!text.empty())
                say(text);
            if(onRespond != nullptr)
                onRespond(text);
        }
    };
    map<string, shared_ptr<Response>> responses;
    vector<unique_ptr<SpeechBubble>> speechBubbles;
    shared_ptr<AIPattern> ai;

public:
    NPC(string name, const fs::path &model=""):Entity(name, model), ai(nullptr) {}
    virtual void onSpeak(Entity *other, string msg) override;
    void addResponse(string text, string response, function<void(string)> onRespond = nullptr) {responses[text] = shared_ptr<Response>(new Response(text, onRespond));}
    void say(string text);
    
    void setAIPattern(shared_ptr<AIPattern> ai) {this->ai = ai; ai->possess(this);}
    
    void label(Camera cam) override;
    void update() override;
};

#endif /* npc_hpp */

//
//  partymember.hpp
//  pool
//
//  Created by Bryce on 8/20/16.
//
//

#ifndef partymember_hpp
#define partymember_hpp

#include <string>
#include <stdio.h>
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "widget.hpp"
#include "speechbubble.hpp"
#include "conversation.hpp"
#include "contextmenu.hpp"
#include "overlay.hpp"
#include "properties.h"
#include "entity.hpp"

using namespace ci;
using namespace std;

class PartyMember : public Widget {
protected:
    class Hand : public Overlay {
        int bobFrame;
    public:
        Hand(const fs::path image="", Rectf rect=Rectf())
        :Overlay(image, rect),
        bobFrame(0)
        {
        }
        
        void onMove(vec3 dir, vec3 vel);
    } hand;
    
    string name;
    gl::TextureRef portrait;
    
    vector<unique_ptr<SpeechBubble>> speechBubbles;
    ContextMenu combatActions;
    
    shared_ptr<Conversation> convo;
    shared_ptr<Entity> entity;
    
    bool drawShadow;
    vec2 shadowPos;
public:
    PartyMember(shared_ptr<Entity> e, string name, const fs::path portrait, const fs::path handImg="");
    
    void draw() override;
    bool onMouseDown(MouseEvent event) override;
    bool onMouseUp(MouseEvent event) override;
    bool onMouseDrag(MouseEvent event) override;

    void joinConversation(shared_ptr<Conversation> convo);
    void say(string msg);
    
    void onMove(vec3 dir, vec3 vel) {hand.onMove(dir, vel);}
    
    void setHandPos(ivec2 pos) {hand.setPos(pos);}
    ivec2 getHandPos() {return hand.getPos();}
    
    shared_ptr<ContextMenu> getCombatMenu() {return shared_ptr<ContextMenu>(&combatActions);}
};

#endif /* partymember_hpp */

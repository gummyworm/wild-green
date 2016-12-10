//
//  partymember.cpp
//  pool
//
//  Created by Bryce on 8/20/16.
//
//

#include "partymember.hpp"
#include "properties.h"
#include "game.h"
#include "entity_widget.hpp"

using namespace ci;
using namespace ci::app;

PartyMember::PartyMember(shared_ptr<Entity> e, string name, const fs::path portrait, const fs::path handImage)
:Widget(nullptr, 32, 64),
name(name),
hand(handImage, properties::playerHandRect),
drawShadow(false),
entity(e)
{
    this->portrait = gl::Texture::create(loadImage(loadAsset(portrait)));
    this->portrait->setMinFilter(GL_NEAREST);
    this->portrait->setMagFilter(GL_NEAREST);
}

void PartyMember::draw()
{
    Widget::draw();
    
    // draw the portrait
    gl::color(1, 1, 1);
    gl::draw(portrait, vec2());
    gl::color(1, 0, 0);
    gl::drawString(name, vec2(0,  -fontSize), fontColor, properties::font);
    
    // and its shadow
    if(drawShadow) {
        gl::draw(portrait, shadowPos);
    }
    
    // draw speech bubble
    for(auto& s : speechBubbles) {
        s->draw();
        s->apply();
    }
    
    // draw the hand
    hand.draw();
}

bool PartyMember::onMouseDown(MouseEvent event)
{
    if(contains(event.getPos())) {
        //drawShadow = true;
        
        auto ew = shared_ptr<EntityWidget>(new EntityWidget(entity, portrait));
        game::guiMgr.addWidget(ew);
        ew->grab(event.getPos() - getAbsPos2D());
        ew->setAbsPos2D(getAbsPos2D());
        shadowPos = event.getPos() - getAbsPos2D();
        if(event.isRight())
            game::speechMgr.silence(entity.get()); //speechBubbles = vector<unique_ptr<SpeechBubble>>();
        else
            say("Hello!");
        return true;
    }
    
    if(event.isRight()) {
        for(auto& s : speechBubbles) {
            if(contains(event.getPos())) {
                s->setVisible(false);
                return true;
            }
        }
        /* TODO: delete from vector
        std::remove_if(speechBubbles.begin(), speechBubbles.end(),
                                      [event](SpeechBubble *s) mutable
                                      {
                                          return s->getRect().contains(event.getPos());
                                    });
         */
    } else {
        for(auto& s : speechBubbles) {
            if(s->onMouseDown(event))
                return true;
        }
    }
    
    return false;
}

bool PartyMember::onMouseUp(MouseEvent event)
{
    drawShadow = false;
    for(auto& s : speechBubbles) {
        if(s->onMouseUp(event))
            return true;
    }
    return false;
}

bool PartyMember::onMouseDrag(MouseEvent event)
{
    for(auto& s : speechBubbles) {
        if(s->onMouseDrag(event))
            return true;
    }
    shadowPos = event.getPos() - getAbsPos2D();
    return false;
}

void PartyMember::joinConversation(shared_ptr<Conversation> convo)
{
    //convo->addSpeaker(make_shared<SpeechBubble>(speech));
    this->convo = convo;
}

void PartyMember::say(string msg)
{
    auto s = unique_ptr<SpeechBubble2>(new SpeechBubble2(this, msg, entity.get()));
    game::speechMgr.say(move(s));
    //speechBubbles.push_back(move(s));
}

void PartyMember::Hand::onMove(vec3 dir, vec3 vel)
{
    // bob the rectangle
    float bob = properties::handBobAmplitude*sin(bobFrame/100.0f);
    rect.y1 = properties::playerHandRect.y1 + bob;
    rect.y2 = properties::playerHandRect.y2 + bob;
    bobFrame += 30;
}
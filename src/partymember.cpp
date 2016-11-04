//
//  partymember.cpp
//  pool
//
//  Created by Bryce on 8/20/16.
//
//

#include "partymember.hpp"
#include "properties.h"
using namespace ci;
using namespace ci::app;

PartyMember::PartyMember(shared_ptr<Entity> e, string name, const fs::path portrait, const fs::path handImage)
:Widget(32,64),
name(name),
hand(handImage, properties::playerHandRect)
{
    Rectf rect = getRect();
    this->portrait = gl::Texture::create(loadImage(loadAsset(portrait)));
    this->portrait->setMinFilter(GL_NEAREST);
    this->portrait->setMagFilter(GL_NEAREST);
    rect = Rectf(50,50,100,100);
    
    this->entity = entity;
}

void PartyMember::draw()
{
    // draw the portrait
    Rectf rect = getRect();
    gl::color(1, 1, 1);
    gl::draw(portrait, rect);
    gl::color(1, 0, 0);
    gl::drawString(name, vec2(rect.x1, rect.y1 - fontSize), fontColor, properties::font);
    
    // draw speech bubble
    for(auto& s : speechBubbles)
        s->draw();
    
    // draw the hand
    hand.draw();
}

void PartyMember::onMouseDown(MouseEvent event)
{
    if(getRect().contains(event.getPos())) {
        if(event.isRight())
            speechBubbles = vector<unique_ptr<SpeechBubble>>();
        else
            say("Hello!");
    }
    
    if(event.isRight()) {
        for(auto& s : speechBubbles) {
            if(s->getRect().contains(event.getPos()))
                s->setVisible(false);
        }
        /* TODO: delete from vector
        std::remove_if(speechBubbles.begin(), speechBubbles.end(),
                                      [event](SpeechBubble *s) mutable
                                      {
                                          return s->getRect().contains(event.getPos());
                                    });
         */
    } else {
        for(auto& s : speechBubbles)
            s->onMouseDown(event);
    }
}

void PartyMember::onMouseUp(MouseEvent event)
{
    for(auto& s : speechBubbles) {
        s->onMouseUp(event);
    }
}

void PartyMember::onMouseDrag(MouseEvent event)
{
    for(auto& s : speechBubbles) {
        s->onMouseDrag(event);
    }
}

void PartyMember::joinConversation(shared_ptr<Conversation> convo)
{
    //convo->addSpeaker(make_shared<SpeechBubble>(speech));
    this->convo = convo;
}

void PartyMember::say(string msg)
{
    auto s = unique_ptr<SpeechBubble2>(new SpeechBubble2(this, msg, entity.get()));
    s->say(msg);
    speechBubbles.push_back(move(s));
}

void PartyMember::Hand::onMove(vec3 dir, vec3 vel)
{
    // bob the rectangle
    float bob = properties::handBobAmplitude*sin(bobFrame/100.0f);
    rect.y1 = properties::playerHandRect.y1 + bob;
    rect.y2 = properties::playerHandRect.y2 + bob;
    bobFrame += 30;
}
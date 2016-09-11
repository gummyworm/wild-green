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

PartyMember::PartyMember(string name, const fs::path portrait)
:name(name),
speech(SpeechBubble(this))
{
    Rectf rect = getRect();
    this->portrait = gl::Texture::create(loadImage(loadAsset(portrait)));
    rect = Rectf(50,50,100,100);
}

void PartyMember::draw()
{
    Rectf rect = getRect();
    gl::color(1, 1, 1);
    gl::draw(portrait, rect);
    gl::color(1, 0, 0);
    gl::drawString(name, vec2(rect.x1, rect.y1 - fontSize), fontColor, properties::font);
    speech.say("test");
    speech.draw();
}

void PartyMember::joinConversation(Conversation *convo)
{
    convo->addSpeaker(shared_ptr<SpeechBubble>(&speech));
}

void PartyMember::say(string msg)
{
    speech.say(msg);
    speech.setVisible(true);
}
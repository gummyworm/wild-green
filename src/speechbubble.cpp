//
//  speechbubble.cpp
//  digital
//
//  Created by Bryce on 9/4/16.
//
//

#include "game.h"
#include "speechbubble.hpp"
#include "properties.h"

SpeechBubble::SpeechBubble(Widget *speakerPortrait, string text, Entity *speaker, bool direction, float fadeTime)
:Widget(),
speakerPortrait(speakerPortrait),
speaker(speaker),
text(text),
dir(direction),
fadeTime(fadeTime),
fadeTimer()
{
    setVisible(!text.empty());
    font = gl::TextureFont::create(Font(properties::speechboxFont));
    
    if(!text.empty())
        say(text);
}

void SpeechBubble::say(string msg)
{
    text = msg;
    setVisible(true);
    
    if(fadeTime > 0.0f)
        fadeTimer.start();
}

void SpeechBubble::update()
{
    if(fadeTime > 0.0f && fadeTimer.getSeconds() > fadeTime)
        setVisible(false);
}

void SpeechBubble::onMouseDown(MouseEvent event)
{
    if(getRect().contains(event.getPos()))
        grab(event.getPos()-ivec2(getRect().getUpperLeft()));
}

void SpeechBubble::onMouseUp(MouseEvent event)
{
    bool startConversation = grabbed && event.isLeft();
    
    ungrab();
    
    if(startConversation) {
        auto target = game::getPicked(event.getPos());
        if(target != nullptr) {
            target->onSpeak(speaker, text);
        }
    }
    
    if(event.isRight() && getRect().contains(event.getPos()))
        setVisible(false);
}

void SpeechBubble::onMouseDrag(MouseEvent event)
{
    if(!grabbed)
        return;
    setPos(event.getPos()-grabbedPos);
    auto e = game::getPicked(event.getPos());
    if(e != nullptr) {
        hovered = e;
        e->setHighlighted(true);
    } else {
        if(hovered != nullptr)
            hovered->setHighlighted(false);
        hovered = nullptr;
    }
}

SpeechBubble2::SpeechBubble2(Widget *speakerPortrait, string text, Entity *speaker, bool direction)
:SpeechBubble(speakerPortrait, text, speaker, direction)
{
    
}

void SpeechBubble2::draw()
{
    Widget::draw();
    if(!show)
        return;
    
    if(speakerPortrait == nullptr)
        return;
    
    gl::color(properties::speechboxBorderColor);
    gl::drawStrokedRect(getRect(), properties::speechboxBorderWidth);
    gl::drawLine(vec3(getRect().x1+8, getRect().y2, 0.0f), vec3(speakerPortrait->getPos())+vec3(3,0,0));
    gl::color(properties::speechboxBgColor);
    gl::enableAlphaBlending();
    gl::drawSolidRect(getRect());
    
    font->drawString(text, vec2(getRect().x1, getRect().y1+properties::speechboxFont.getSize()/2) + vec2(properties::speechboxTextOffset));
}

void SpeechBubble2::say(string msg)
{
    SpeechBubble::say(msg);
    
    vec2 size = font->measureString(text) + vec2(properties::speechboxTextOffset);
    
    float top, bot, left, right;
    switch (dir) {
        case UP:
            top = speakerPortrait->getPos2D().y-size.y-(properties::speechboxOffset.y);
            bot = speakerPortrait->getPos2D().y-properties::speechboxOffset.y+properties::speechboxTextOffset.y;
            left = speakerPortrait->getPos2D().x+properties::speechboxOffset.x;
            right = speakerPortrait->getPos2D().x+size.x+properties::speechboxOffset.x+properties::speechboxTextOffset.x;
            break;
        case DOWN:
            top = speakerPortrait->getPos2D().y;
            bot = speakerPortrait->getPos2D().y+properties::speechboxSize.y;
            left = speakerPortrait->getPos2D().x;
            right = speakerPortrait->getPos2D().x+properties::speechboxSize.x;
            break;
        default:
            break;
    }
    
    setPos(ivec2(left, top));
    resize(ivec2(right-left, bot-top));
    rect = Rectf(left, top, right, bot);
}

SpeechBubble3::SpeechBubble3(Entity *speaker, string text, bool direction)
:SpeechBubble(nullptr, text, speaker, direction)
{
    bgColor = ColorA(1,1,1,1);
    fontColor = ColorA(0,0,0,1);
}

void SpeechBubble3::update()
{
    vec2 size = font->measureString(text);
    vec2 line = mainCam.worldToScreen(speaker->getAABB().getMax() * speaker->getScale() + speaker->getPos(), properties::screenWidth, properties::screenHeight) - vec2(0, size.y);
    this->rect = Rectf(line.x, line.y-size.y, line.x+size.x, line.y+size.y);
}

void SpeechBubble3::draw()
{
    if(!show)
        return;
    if(mainCam.worldToEyeDepth(pos) > 0)
        return;
    
    gl::pushModelMatrix();
    gl::setMatricesWindow(properties::screenSize);
    
    vec2 size = font->measureString(text);
    vec2 line = mainCam.worldToScreen(speaker->getAABB().getMax() * speaker->getScale() + speaker->getPos(), properties::screenWidth, properties::screenHeight) - vec2(0, size.y);
    Rectf rect(line.x, line.y-size.y, line.x+size.x, line.y+size.y);
    
    game::uiProg->bind();
    gl::color(properties::speechboxBorderColor);
    gl::drawStrokedRect(rect, properties::speechboxBorderWidth);
    gl::color(bgColor);
    gl::enableAlphaBlending();
    gl::disable(GL_TEXTURE_2D);
    //gl::drawSolidRect(rect, vec2(0,0), vec2(0,0));
    gl::drawSolidRoundedRect(rect, 0);  // TODO: why
    //gl::drawLine(vec3(rect.x1+8, rect.y2, 0.0f), vec3((rect.x2+rect.x1)/2.0f, rect.y1, 0)+vec3(3,0,0)); //TODO:
    gl::color(fontColor);
    font->drawString(text, line);
    
    gl::popModelMatrix();
}

void SpeechBubble3::say(string msg)
{
    text = msg;
}
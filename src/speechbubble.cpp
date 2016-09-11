//
//  speechbubble.cpp
//  digital
//
//  Created by Bryce on 9/4/16.
//
//

#include "speechbubble.hpp"
#include "properties.h"

SpeechBubble::SpeechBubble(Widget *speaker, string text, bool direction)
:Widget(),
speaker(speaker),
text(text),
dir(direction)
{
    setVisible(!text.empty());
}

void SpeechBubble::say(string msg)
{
    text = msg;
    setVisible(true);
}

void SpeechBubble::draw()
{
    if(speaker == nullptr)
        return;
    
    float top, bot, left, right;
    switch (dir) {
        case UP:
            top = speaker->getPos2D().y-properties::speechboxSize.y;
            bot = speaker->getPos2D().y;
            left = speaker->getPos2D().x;
            right = properties::speechboxSize.x;
            break;
        case DOWN:
            top = speaker->getPos2D().y;
            bot = speaker->getPos2D().y+properties::speechboxSize.y;
            left = speaker->getPos2D().x;
            right = properties::speechboxSize.x;
            break;
        default:
            break;
    }
    Rectf r(left, top, right, bot);
    gl::color(properties::speechboxBgColor);
    gl::drawSolidRect(r);
    gl::drawString(text, vec2(left, top), properties::speechboxTextColor);
}
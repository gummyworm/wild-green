//
//  button.cpp
//  digital
//
//  Created by Bryce on 11/2/16.
//
//

#include "button.hpp"
#include "properties.h"

Button::Button(Widget *parent, function<void()> onClick)
:Widget(parent),
pressedColor(0.6f, 0.6f, 0.6f, 1),
releasedColor(0.8f, 0.8f, 0.8f, 1),
onClick(onClick)
{
    flags.pressed = false;
}

void Button::draw()
{
    Widget::draw();
    if(flags.pressed) {
        gl::color(pressedColor);
    } else {
        gl::color(releasedColor);
    }
    gl::drawSolidRect(getRect());
}

bool Button::onMouseDown(MouseEvent event)
{
    if(contains(event.getPos())) {
        flags.pressed = true;
        if(onClick != nullptr)
            onClick();
        return true;
    }
    return false;
}

bool Button::onMouseUp(MouseEvent event)
{
    bool handled = !flags.pressed && contains(event.getPos());
    flags.pressed = false;
    if(handled) {
        if(onClick != nullptr)
            onClick();
        return true;
    }
    return handled;
}

ToggleButton::ToggleButton(Widget *parent, function<void()> onClick)
:Button(parent, onClick)
{
}

bool ToggleButton::onMouseDown(MouseEvent event)
{
    if(contains(event.getPos())) {
        flags.pressed = !flags.pressed;
        return true;
    }
    return false;
}

bool ToggleButton::onMouseUp(MouseEvent event)
{
    return contains(event.getPos());
}

void ToggleButton::draw()
{
    Widget::draw();
    if(flags.pressed) {
        gl::color(pressedColor);
    } else {
        gl::color(releasedColor);
    }
    gl::drawSolidRect(getRect());
}

TextButton::TextButton(Widget *parent, string text, function<void()> clicked)
:Button(parent, clicked),
text(text)
{
    font = gl::TextureFont::create(properties::font);
    resize(font->measureString(text));
}

void TextButton::draw()
{
    Button::draw();
    gl::drawString(text, getRect().getUpperLeft());
}
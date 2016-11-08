//
//  button.cpp
//  digital
//
//  Created by Bryce on 11/2/16.
//
//

#include "button.hpp"

Button::Button()
:Widget(),
pressed(false),
pressedColor(0.6f, 0.6f, 0.6f, 1),
releasedColor(0.8f, 0.8f, 0.8f, 1)
{
}

void Button::draw()
{
    Widget::draw();
    if(pressed)
        gl::color(pressedColor);
    else
        gl::color(releasedColor);
    gl::drawSolidRect(getRect());
}

bool Button::onMouseDown(MouseEvent event)
{
    if(contains(event.getPos())) {
        pressed = true;
        return true;
    }
    return false;
}

bool Button::onMouseUp(MouseEvent event)
{
    bool handled = !pressed && contains(event.getPos());
    pressed = false;
    return handled;
}

TextButton::TextButton(string text)
:Button(),
text(text)
{
}

void TextButton::draw()
{
    Button::draw();
    gl::drawString(text, getRect().getUpperLeft());
}
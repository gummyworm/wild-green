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
releasedColor(0.6f, 0.6f, 0.6f, 1)
{
}

void Button::draw()
{
    if(pressed)
        gl::color(pressedColor);
    else
        gl::color(releasedColor);
    gl::drawSolidRect(getRect());
}

void Button::onMouseDown(MouseEvent event)
{
    pressed = true;
}

void Button::onMouseUp(MouseEvent event)
{
    pressed = false;
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
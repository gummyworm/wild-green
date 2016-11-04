//
//  cursor.cpp
//  digital
//
//  Created by Bryce on 9/22/16.
//
//

#include "cursor.hpp"
#include "properties.h"

Cursor::Cursor(const fs::path image)
{
    this->tex = gl::Texture::create(loadImage(loadAsset(image)));
    rect = Rectf(vec2(), properties::cursorSize);
}

Cursor::Cursor()
:tex(nullptr)
{
}

void Cursor::draw()
{
    if(tex == nullptr)
        return;
    gl::color(1, 1, 1);
    gl::draw(tex, rect);
    gl::color(1, 0, 0);
}

void Cursor::onMouseMove(MouseEvent event)
{
    rect = Rectf(event.getPos(), event.getPos()+properties::cursorSize);
}
//
//  overlay.cpp
//  digital
//
//  Created by Bryce on 9/22/16.
//
//

#include "overlay.hpp"

using namespace ci;
using namespace ci::app;

Overlay::Overlay(const fs::path image, Rectf rect)
:tex(nullptr),
rect(rect)
{
    setImage(image);
}

void Overlay::setImage(const fs::path image)
{
    if(!image.empty())
        tex = gl::Texture::create(loadImage(loadAsset(image)));
}

void Overlay::draw()
{
    if(tex == nullptr)
        return;
    gl::color(1, 1, 1);
    gl::draw(tex, rect);
    gl::color(1, 0, 0);
}

void Overlay::setPos(ivec2 pos)
{
    rect.x2 = rect.getWidth() + pos.x;
    rect.y2 = rect.getHeight() + pos.y;
    rect.x1 = pos.x;
    rect.y1 = pos.y;
}
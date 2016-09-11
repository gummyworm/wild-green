//
//  prg.cpp
//  digital
//
//  Created by Bryce on 8/26/16.
//
//

#include "prg.hpp"

PRGLauncher::PRGLauncher(string name, string icon)
:Widget(),
name(name)
{
    if(icon.empty()) {
        icon = "geos-kernal.png";
    }
    auto img = loadImage(loadAsset(icon));
    this->icon.tex = gl::Texture::create(img);
    setPos(vec2(100,100));
}

void PRGLauncher::draw()
{
    gl::draw(icon.tex, vec2(pos.x, pos.y));
    for(auto i : instances) {
        i->draw();
    }
}

void PRGLauncher::onMouseDown(MouseEvent event)
{
    for(auto i : instances) {
        i->onMouseDown(event);
    }
}

void PRGLauncher::onMouseDrag(MouseEvent event)
{
    for(auto i : instances) {
        i->onMouseDrag(event);
    }
}

void PRGLauncher::onAccept(MouseEvent event, shared_ptr<class Entity> e)
{
    for(auto i : instances) {
        i->onAccept(event, e);
    }
}

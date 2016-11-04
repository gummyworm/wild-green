//
//  prg.cpp
//  digital
//
//  Created by Bryce on 8/26/16.
//
//

#include "prg.hpp"
#include "properties.h"

PRGLauncher::PRGLauncher(string name, string icon)
:Widget(),
name(name),
grabbed(false)
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
    if(grabbed)
        gl::color(properties::iconGrabbedColor);
    else
        gl::color(1, 1, 1);
    
    gl::draw(icon.tex, vec2(pos.x, pos.y));
    for(auto i : instances) {
        i->draw();
    }
}

void PRGLauncher::onMouseDown(MouseEvent event)
{
    if(getRect().contains(event.getPos())) {
        grabbed = true;
        grabOffset = ivec2(getRect().getUpperLeft()) - event.getPos();
        if(doubleClickTimer.isStopped())
            doubleClickTimer.start();
        else if(doubleClickTimer.getSeconds() <= properties::doubleClickDelay) {
            launch();
            doubleClickTimer.stop();
        } else {
            doubleClickTimer.start();
        }
    }
    
    for(auto i : instances) {
        i->onMouseDown(event);
    }
}

void PRGLauncher::onMouseUp(MouseEvent event)
{
    grabbed = false;
    for(auto i : instances) {
        i->onMouseUp(event);
    }
}

void PRGLauncher::onMouseDrag(MouseEvent event)
{
    if(grabbed) {
        setPos(event.getPos() + grabOffset);
    }
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

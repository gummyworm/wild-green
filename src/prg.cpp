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
    resize(ivec2(this->icon.tex->getWidth(), this->icon.tex->getHeight()));
    
    flags.drawBorder = false;
}

void PRGLauncher::draw()
{
    Widget::draw();
    if(grabbed)
        gl::color(properties::iconGrabbedColor);
    else
        gl::color(1, 1, 1, 1);
    gl::draw(icon.tex, vec2());
}

bool PRGLauncher::onMouseDown(MouseEvent event)
{
    if(contains(event.getPos())) {
        grabbed = true;
        grabOffset = ivec2(getPos2D() - event.getPos());
        if(doubleClickTimer.isStopped())
            doubleClickTimer.start();
        else if(doubleClickTimer.getSeconds() <= properties::doubleClickDelay) {
            launch();
            doubleClickTimer.stop();
        } else {
            doubleClickTimer.start();
        }
        return true;
    }
    return false;
}

bool PRGLauncher::onMouseUp(MouseEvent event)
{
    grabbed = false;
    return false;
}

bool PRGLauncher::onMouseDrag(MouseEvent event)
{
    if(grabbed) {
        setPos(event.getPos() + grabOffset);
        return true;
    }
    return false;
}

bool PRGLauncher::onAccept(MouseEvent event, shared_ptr<class Entity> e)
{
    return false;
}

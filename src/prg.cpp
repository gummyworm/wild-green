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
    setPos(vec3(100,100,-100));
    resize(ivec2(this->icon.tex->getWidth(), this->icon.tex->getHeight()));
    
    flags.drawBorder = false;
    flags.drawBg = false;
    flags.scissor = false;
    
    font = gl::TextureFont::create(properties::iconFont);
    fontColor = ColorA(1,0.5,1,1);
}

void PRGLauncher::draw()
{
    Widget::draw();
    if(grabbed)
        gl::color(properties::iconGrabbedColor);
    else
        gl::color(1, 1, 1, 1);
    gl::draw(icon.tex, vec2());
    
    // draw the name
    gl::color(0, 0, 0, 1);
    gl::drawSolidRect(Rectf(vec2(-2, icon.tex->getHeight()), vec2(0, icon.tex->getHeight()) + font->measureString(name) + vec2(2,4)));
    gl::color(fontColor);
    font->drawString(name, vec2(0, icon.tex->getHeight() + font->measureString(name).y));
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

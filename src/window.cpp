//
//  window.cpp
//  digital
//
//  Created by Bryce on 8/25/16.
//
//

#include "game.h"
#include "gui.h"
#include "window.hpp"
#include "properties.h"

void WidgetWindow::Scrollbar::draw(Rectf parentRect)
{
    //if(!enabled)
    //return;
    
    Rectf leftarrowRect = getLeftArrow();
    Rectf rightarrowRect = getRightArrow();
    Rectf uparrowRect = getUpArrow();
    Rectf downarrowRect = getDownArrow();
    Rectf barRect = getBarRect();
    if(horiz) {
        gl::drawSolidRect(barRect);
        gl::draw(leftArrow, leftarrowRect);
        gl::draw(rightArrow, rightarrowRect);
    } else {
        gl::drawSolidRect(barRect);
        gl::draw(upArrow, uparrowRect);
        gl::draw(downArrow, downarrowRect);
    }
}

Rectf WidgetWindow::Scrollbar::getLeftArrow()
{
    Rectf r = getBarRect();
    r.x2 = r.x1 + gui::windowBorderWidth;
    return r;
}

Rectf WidgetWindow::Scrollbar::getRightArrow()
{
    Rectf r = getBarRect();
    r.x1 = r.x2 - gui::windowBorderWidth;
    return r;
}

Rectf WidgetWindow::Scrollbar::getDownArrow()
{
    Rectf r = getBarRect();
    r.y1 = r.y2 - gui::windowBorderWidth;
    return r;
}

Rectf WidgetWindow::Scrollbar::getUpArrow()
{
    Rectf r = getBarRect();
    r.y2 = r.y1 + gui::windowBorderWidth;
    return r;
}

Rectf WidgetWindow::Scrollbar::getBarRect()
{
    Rectf r = parent->getRect();
    if(horiz) {
        r.x2 -= gui::windowBorderWidth;
        r.y1 = r.y2 - gui::windowBorderWidth;
    } else {
        r.y1 += properties::windowTitleHeight;
        r.y2 -= gui::windowBorderWidth;
        r.x1 = r.x2 - gui::windowBorderWidth;
    }
    return r;
}

Rectf WidgetWindow::getResizeRect()
{
    Rectf r = getRect();
    r.x1 = r.x2 - gui::windowBorderWidth;
    r.y1 = r.y2 - gui::windowBorderWidth;
    return r;
}

Rectf WidgetWindow::getBringFrontRect()
{
    Rectf r = getRect();
    r.x2 -= gui::windowBorderWidth;
    r.x1 = r.x2 - gui::windowBorderWidth;
    r.y2 = r.y1 + gui::windowBorderWidth;
    return r;
}

void WidgetWindow::Scrollbar::onMouseDown(MouseEvent event, Rectf winRect)
{
    Rectf offset = Rectf(parent->getAbsPos2D(), parent->getAbsPos2D());
    Rectf leftarrowRect = getLeftArrow() + offset;
    Rectf rightarrowRect = getRightArrow() + offset;
    Rectf uparrowRect = getUpArrow() + offset;
    Rectf downarrowRect = getDownArrow() + offset;
    
    ivec2 pos = event.getPos();
    
    if(horiz) {
        if(leftarrowRect.contains(pos) && scroll > 0) {
            scroll--;
        } else if(rightarrowRect.contains(pos) && scroll < parent->getVirtualSize().x) {
            scroll++;
        }
    }
    else {
        if(uparrowRect.contains(pos) && scroll > 0) {
            scroll--;
        } else if(downarrowRect.contains(pos) && scroll < parent->getVirtualSize().y) {
            scroll++;
        }
    }
}

WidgetWindow::WidgetWindow(string name, int width, int height)
:Widget(nullptr, width, height),
title(name),
grabbed(false),
resizing(false),
prevMouse(0,0),
closeButton(3,3,10,10),
closeButtonOffset(3,3),
resizable(true),
hScroll(this, true),
vScroll(this, false)
{
    setVisible(true);
    resize(ivec2(width, height));
    resizeIcon = gl::Texture::create(loadImage(loadAsset("resizeicon.png")));
    bringFrontIcon = gl::Texture::create(loadImage(loadAsset("bringfronticon.png")));
    
    flags.scissor = true;
    virtualSize = ivec2(1000,1000);
}

bool WidgetWindow::onMouseDown(MouseEvent event)
{    
    Rectf grabRect(vec2(), vec2(dim.x, title.height));
    
    vec2 mousePos = event.getPos();
    vec2 mouseRel = mousePos - vec2(getPos2D());
    
    // check close
    if(closeButton.contains(mouseRel)) {
        game::guiMgr.remove(this);
        return true;
    }
    // check scroll bar(s)
    vScroll.onMouseDown(event, getRect());
    hScroll.onMouseDown(event, getRect());
    
    // check bring front
    if(getBringFrontRect().contains(mouseRel)) {
        game::guiMgr.bringFront(this);
        return true;
    }
    
    // check if grab
    if(grabRect.contains(mouseRel)) {
        prevMouse = mousePos;
        grabbed = true;
        return true;
    } else {
        grabbed = false;
    }
    
    // check resize
    if(resizable && getResizeRect().contains(mouseRel)) {
        prevMouse = mousePos;
        resizing = true;
        return true;
    } else {
        resizing = false;
    }

    event.setPos(event.getPos() + getScroll());
    for(auto c : children) {
        if(c->onMouseDown(event)) {
            return true;
        }
    }
    return false;
}

bool WidgetWindow::onMouseUp(MouseEvent event)
{
    grabbed = false;
    resizing = false;
    
    Widget::onMouseUp(event);
    
    event.setPos(event.getPos() + getScroll());
    for(auto c : children) {
        if(c->onMouseUp(event)) {
            return true;
        }
    }
    return false;
}

bool WidgetWindow::onMouseMove(MouseEvent event)
{
    event.setPos(event.getPos() + getScroll());
    return Widget::onMouseMove(event);
}

bool WidgetWindow::onMouseDrag(MouseEvent event)
{
    if(resizable && resizing) {
        dim.x += event.getX() - prevMouse.x;
        dim.y += event.getY() - prevMouse.y;
        
        if(dim.x > maxDim.x)
            dim.x = maxDim.x;
        if(dim.y > maxDim.y)
            dim.y = maxDim.y;
        if(dim.x < minDim.x)
            dim.x = minDim.x;
        if(dim.y < minDim.y)
            dim.y = minDim.y;
        
        prevMouse = event.getPos();
        return true;
    }
    
    if(!grabbed) {
        return false;
    }
    
    ivec2 mousePos = event.getPos();
    int dx = mousePos.x - prevMouse.x;
    int dy = mousePos.y - prevMouse.y;
    setPos(getPos() + ivec3(dx, dy, 0));
    
    closeButton.x1 = closeButtonOffset.x;
    closeButton.y1 = closeButtonOffset.y;
    closeButton.x2 = closeButtonOffset.x + 7;
    closeButton.y2 = closeButtonOffset.y + 7;
    prevMouse = mousePos;
    return true;
}

bool WidgetWindow::onAccept(MouseEvent event, shared_ptr<class Entity> e)
{
    Widget::onAccept(event, e);
    event.setPos(event.getPos() + getScroll());
    
    for(auto c : children) {
        if(c->onAccept(event, e)) {
            return true;
        }
    }
    return false;
}

void WidgetWindow::setTitle(string title)
{
    this->title.text = title;
}

void WidgetWindow::draw()
{
    Widget::draw();
    if(!show)
        return;
    
    vec2 titleDim = title.font->measureString(title.text);
    vec2 titleLoc = vec2(dim.x/2.0f - titleDim.x/2.0f, titleDim.y);
    Rectf titleRect = getRect();
    titleRect.y2 = titleRect.y1 + title.height;
    
    gl::color(1,0,0,1);
    
    gl::color(gui::windowBgColor);
    gl::color(0,0,1,1);
    gl::drawSolidRect(titleRect);
    gl::color(1,1,1,1);
    gl::drawSolidRect(closeButton);
    title.font->drawString(title.text, titleLoc);
    
    if(resizable) {
        gl::draw(resizeIcon, getResizeRect());
    }
    
    gl::draw(bringFrontIcon, getBringFrontRect());
    
    hScroll.draw(getRect());
    vScroll.draw(getRect());
    
    gl::color(1,1,1,1);
    
    if(virtualSize.x > getRect().getWidth()) {
        hScroll.draw(getRect());
    }
    if(virtualSize.y > getRect().getHeight()) {
        vScroll.draw(getRect());
    }
    
    gl::translate(-hScroll.getScroll(), -vScroll.getScroll(), 0);
    
    vec2 scale = vec2((float)app::getWindowWidth() / properties::screenWidth, (float)app::getWindowHeight() / properties::screenHeight);
    vec2 d = vec2(dim.x - gui::windowBorderWidth, dim.y - gui::windowBorderWidth - title.height);
    int x = getAbsPos2D().x;
    int y = properties::screenHeight - (getAbsPos2D().y + (dim.y - gui::windowBorderWidth));
    gl::scissor(scale.x * x, scale.y * y, d.x * scale.x, d.y * scale.y);
    for(auto c : children) {
        c->draw();
        c->apply();
    }
}

Rectf WidgetWindow::getInternalRect()
{
    Rectf r = getRect() + Rectf(0, title.height, 0, 0);
    if(hScroll.getVisible()) {
        r.y2 -= gui::borderWidth;
    }
    if(vScroll.getVisible()) {
        r.x2 -= gui::borderWidth;
    }
    return r;
}

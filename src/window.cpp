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
    
    Rectf leftarrowRect = getLeftArrow(parentRect);
    Rectf rightarrowRect = getRightArrow(parentRect);
    Rectf uparrowRect = getUpArrow(parentRect);
    Rectf downarrowRect = getDownArrow(parentRect);
    Rectf barRect = getBarRect(parentRect);
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

Rectf WidgetWindow::Scrollbar::getLeftArrow(Rectf winRect)
{
    Rectf r = getBarRect(winRect);
    r.x2 = r.x1 + gui::windowBorderWidth;
    return r;
}

Rectf WidgetWindow::Scrollbar::getRightArrow(Rectf winRect)
{
    Rectf r = getBarRect(winRect);
    r.x1 = r.x2 - gui::windowBorderWidth;
    return r;
}

Rectf WidgetWindow::Scrollbar::getDownArrow(Rectf winRect)
{
    Rectf r = getBarRect(winRect);
    r.y1 = r.y2 - gui::windowBorderWidth;
    return r;
}

Rectf WidgetWindow::Scrollbar::getUpArrow(Rectf winRect)
{
    Rectf r = getBarRect(winRect);
    r.y2 = r.y1 + gui::windowBorderWidth;
    return r;
}

Rectf WidgetWindow::Scrollbar::getBarRect(Rectf winRect)
{
    Rectf r = winRect;
    if(horiz) {
        r.y1 = r.y2;
        r.y2 += gui::windowBorderWidth;
    } else {
        r.x1 = r.x2;
        r.x2 += gui::windowBorderWidth;
    }
    return r;
}

Rectf WidgetWindow::getResizeRect(Rectf winRect)
{
    Rectf r = winRect;
    r.x1 = r.x2;
    r.x2 += gui::windowBorderWidth;
    r.y1 = r.y2;
    r.y2 += gui::windowBorderWidth;
    return r;
}

Rectf WidgetWindow::getBringFrontRect(Rectf winRect)
{
    Rectf r = winRect;
    r.x1 = r.x2;
    r.x2 += gui::windowBorderWidth;
    r.y2 = r.y1;
    r.y1 -= gui::windowBorderWidth;
    return r;
}

void WidgetWindow::Scrollbar::onMouseDown(MouseEvent event, Rectf winRect)
{
    Rectf leftarrowRect = getLeftArrow(winRect);
    Rectf rightarrowRect = getRightArrow(winRect);
    Rectf uparrowRect = getUpArrow(winRect);
    Rectf downarrowRect = getDownArrow(winRect);
    
    ivec2 pos = event.getPos();
    if(horiz) {
        if(leftarrowRect.contains(pos)) {
            cout << "left" << endl;
        } else if(rightarrowRect.contains(pos)) {
            cout << "right" << endl;
        }
    }
    else {
        if(uparrowRect.contains(pos)) {
            cout << "up" << endl;
        } else if(downarrowRect.contains(pos)) {
            cout << "down" << endl;
        }
    }
}

WidgetWindow::WidgetWindow(string name, int width, int height)
:Widget(width, height),
title(name),
grabbed(false),
resizing(false),
prevMouse(0,0),
closeButton(3,3,10,10),
closeButtonOffset(3,3),
resizable(true),
hScroll(true),
vScroll(false)
{
    setVisible(true);
    resize(ivec2(width, height));
    resizeIcon = gl::Texture::create(loadImage(loadAsset("resizeicon.png")));
    bringFrontIcon = gl::Texture::create(loadImage(loadAsset("bringfronticon.png")));
}

void WidgetWindow::onMouseDown(MouseEvent event)
{
    vec2 mousePos = event.getPos();
    
    // check close
    if(closeButton.contains(mousePos)) {
        setVisible(false);
        return;
    }
    // check scroll bar(s)
    vScroll.onMouseDown(event, getRect());
    hScroll.onMouseDown(event, getRect());
    
    // check bring front
    if(getBringFrontRect(getRect()).contains(mousePos)) {
        game::guiMgr.bringFront(this);
    }
    
    // check if grab
    if(mousePos.y >= pos.y && mousePos.y <= (pos.y + title.height) &&
       mousePos.x >= pos.x && mousePos.x <= (pos.x + dim.x)) {
        prevMouse = mousePos;
        grabbed = true;
        
    } else {
        grabbed = false;
    }
    
    // check resize
    if(getResizeRect(getRect()).contains(mousePos)) {
        prevMouse = mousePos;
        resizing = true;
    } else {
        resizing = false;
    }
}

void WidgetWindow::onMouseUp(MouseEvent event)
{
    grabbed = false;
}

void WidgetWindow::onMouseMove(MouseEvent event)
{
}

void WidgetWindow::onMouseDrag(MouseEvent event)
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
    }
    
    if(!grabbed)
        return;
    
    vec2 mousePos = event.getPos();
    this->pos.x += mousePos.x - prevMouse.x;
    this->pos.y += mousePos.y - prevMouse.y;
    closeButton.x1 = pos.x + closeButtonOffset.x;
    closeButton.y1 = pos.y + closeButtonOffset.y;
    closeButton.x2 = pos.x + closeButtonOffset.x + 7;
    closeButton.y2 = pos.y + closeButtonOffset.y + 7;
    prevMouse = event.getPos();
}


void WidgetWindow::setTitle(string title)
{
    this->title.text = title;
}

void WidgetWindow::draw()
{
    if(!show)
        return;
    
    gl::pushMatrices();
    gl::setMatricesWindow(properties::screenSize);
    
    vec2 titleLoc = vec2(pos.x + (dim.x/2), pos.y + 2);
    Rectf titleRect = getRect();
    titleRect.y2 = titleRect.y1 + title.height;
    
    gl::color(1,0,0);
    gl::color(gui::windowBorderColor);
    
    float bw = gui::windowBorderWidth;
    gl::drawSolidRect(getRect() + Rectf(-bw, -bw, bw, bw));
    gl::color(gui::windowBgColor);
    gl::drawSolidRect(getRect());
    gl::color(0,0,1);
    gl::drawSolidRect(titleRect);
    gl::color(1,1,1);
    gl::drawSolidRect(closeButton);
    gl::drawString(title.text, titleLoc);
    
    if(resizable) {
        gl::draw(resizeIcon, getResizeRect(getRect()));
    }
    
    gl::draw(bringFrontIcon, getBringFrontRect(getRect()));
    
    gl::popMatrices();
    
    hScroll.draw(getRect());
    vScroll.draw(getRect());
    /*
    if(true) {  //virtualSize.x > getRect().getWidth()) {
        hScroll.draw(getRect());
    }
    if(virtualSize.y > getRect().getHeight()) {
        vScroll.draw(getRect());
    }
    */
}

Rectf WidgetWindow::getInternalRect()
{
    Rectf rect = getRect();
    return Rectf(rect.x1, rect.y1 + title.height, rect.x2, rect.y2);
}
//
//  window.cpp
//  digital
//
//  Created by Bryce on 8/25/16.
//
//

#include "window.hpp"
#include "properties.h"

WidgetWindow::WidgetWindow(string name, int width, int height)
:Widget(width, height),
title(name),
grabbed(false),
prevMouse(0,0),
closeButton(3,3,10,10),
closeButtonOffset(3,3)
{
    setVisible(true);
}

void WidgetWindow::onMouseDown(MouseEvent event)
{
    vec2 mousePos = event.getPos();

    // check close
    if(closeButton.contains(mousePos)) {
        setVisible(false);
        return;
    }
    
    // check if grab
    if(mousePos.y >= pos.y && mousePos.y <= (pos.y + title.height) &&
       mousePos.x >= pos.x && mousePos.x <= (pos.x + dim.x)) {
        prevMouse = mousePos;
        grabbed = true;

    } else {
        grabbed = false;
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
    if(!grabbed)
        return;
    vec2 mousePos = event.getPos();
    this->pos.x += mousePos.x - prevMouse.x;
    this->pos.y += mousePos.y - prevMouse.y;
    closeButton.x1 = pos.x + closeButtonOffset.x;
    closeButton.y1 = pos.y + closeButtonOffset.y;
    closeButton.x2 = pos.x + closeButtonOffset.x + 7;
    closeButton.y2 = pos.y + closeButtonOffset.y + 7;

    prevMouse = mousePos;
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
    gl::drawSolidRect(getRect());
    gl::color(0,0,1);
    gl::drawSolidRect(titleRect);
    gl::color(1,1,1);
    gl::drawSolidRect(closeButton);
    gl::drawString(title.text, titleLoc);
    
    gl::popMatrices();

}

Rectf WidgetWindow::getInternalRect()
{
    Rectf rect = getRect();
    return Rectf(rect.x1, rect.y1 + title.height, rect.x2, rect.y2);
}
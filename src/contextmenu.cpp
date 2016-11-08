//
//  contextmenu.cpp
//  digital
//
//  Created by Bryce on 8/23/16.
//
//

#include "contextmenu.hpp"
#include "widget.hpp"
#include "properties.h"

using namespace std;

ContextMenu::ContextMenu()
:Widget(),
highlightColor(0.8f,0.8f,0.8f,1.0f)
{
    flags.drawBorder = false;
}

void ContextMenu::addItem(string name, function<void()> callback)
{
    items.push_back(MenuItem(name, callback));
}

void ContextMenu::addItem(string name, shared_ptr<ContextMenu> submenu)
{
    items.push_back(MenuItem(name, submenu));
}

void ContextMenu::draw()
{
    Widget::draw();
    if(!show)
        return;
    
    Rectf rect = getRect();
    float bot = rect.y2;

    rect.y2 = fontSize;
    vec2 p = vec2();
    
    for(int i = 0; i < items.size(); ++i) {
        auto a = items[i];
        if((p.y+fontSize) > bot)
            break;

        if(i == highlightedItem)
            gl::color(highlightColor);
        else
            gl::color(bgColor);
    
        gl::drawSolidRect(rect);
        gl::drawString(a.getName(), p, fontColor);
        
        p.y += fontSize;
        rect.y1 += fontSize;
        rect.y2 += fontSize;
    }
    
    gl::color(ColorA::white());
}

int ContextMenu::itemOffset(int y)
{
    int offset = y - pos.y;
    if(offset < 0 || offset > dim.y)
        return -1;
    int choice = offset / fontSize;
    if(choice < items.size())
        return choice;
    return -1;
}

bool ContextMenu::onMouseDown(MouseEvent event)
{
    setVisible(true);
    setPos(event.getPos());
    highlightedItem = 0;
    return true;
}

bool ContextMenu::onMouseMove(MouseEvent event)
{
    if(!isVisible())
        return false;
    
    return Widget::onMouseMove(event);
}

bool ContextMenu::onMouseUp(MouseEvent event)
{
    if(!isVisible())
        return false;
    
    setVisible(false);
    vec2 mousePos = event.getPos();
    int choice = itemOffset(mousePos.y);
    if(choice >= 0) {
        auto a = items[choice].getAction();
        if(a != nullptr)
            a();
        return true;
    }
    return false;
}

bool ContextMenu::onMouseDrag(MouseEvent event)
{
    if(!isVisible())
        return false;
    
    vec2 mousePos = event.getPos();
    int choice = itemOffset(mousePos.y);
    if(choice >= 0)
        highlightedItem = choice;
    return true;
}

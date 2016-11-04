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
    if(!show)
        return;
    
    Rectf rect = getRect();
    float bot = rect.y2;
    
    gl::pushMatrices();
    gl::setMatricesWindow(properties::screenSize);

    rect.y2 = rect.y1 + fontSize;
    vec2 p = vec2(pos);
    
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

void ContextMenu::onMouseDown(MouseEvent event)
{
    setVisible(true);
    highlightedItem = 0;
}

void ContextMenu::onMouseMove(MouseEvent event)
{
}

void ContextMenu::onMouseUp(MouseEvent event)
{
    setVisible(false);
    vec2 mousePos = event.getPos();
    int choice = itemOffset(mousePos.y);
    if(choice >= 0) {
        auto a = items[choice].getAction();
        if(a != nullptr)
            a();
    }
}

void ContextMenu::onMouseDrag(MouseEvent event)
{
    vec2 mousePos = event.getPos();
    int choice = itemOffset(mousePos.y);
    if(choice >= 0)
        highlightedItem = choice;
}

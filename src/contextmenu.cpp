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
highlightColor(0.8f,0.8f,0.8f)
{

}

void ContextMenu::addItem(string name, function<void()> callback)
{
    actions.push_back(make_pair(name, callback));
}

void ContextMenu::draw()
{
    if(!show)
        return;
    
    Rectf rect = getRect();
    
    gl::pushMatrices();
    gl::setMatricesWindow(properties::screenSize);
    gl::color(bgColor);
    gl::drawSolidRect(rect);
    
    rect.y2 = rect.y1 + fontSize;

    vec2 p = vec2(pos);
    int i = 0;
    for(auto a : actions) {
        if(p.y > rect.y2)
            break;
        
        if(i == highlightedItem)
            gl::color(highlightColor);
        else
            gl::color(bgColor);
        
        gl::drawSolidRect(rect);
        gl::drawString(a.first, p, fontColor);
        
        p.y += fontSize;
        rect.y1 = p.y;
        rect.y2 = p.y + fontSize;
        
        i++;
    }
    
    gl::color(ColorA::white());
}

int ContextMenu::itemOffset(int y)
{
    int offset = y - pos.y;
    if(offset < 0 || offset > dim.y)
        return -1;
    int choice = offset / fontSize;
    if(choice < actions.size())
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
    if(choice >= 0)
        actions[choice].second();
}

void ContextMenu::onMouseDrag(MouseEvent event)
{
    vec2 mousePos = event.getPos();
    int choice = itemOffset(mousePos.y);
    if(choice >= 0)
        highlightedItem = choice;
}

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
#include "game.h"

using namespace std;

ContextMenu::ContextMenu(string name)
:Widget(),
name(name),
highlightColor(0.8f,0.8f,0.8f,1.0f)
{
    flags.drawBorder = false;
    font = gl::TextureFont::create(properties::menuFont);
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
    vec2 p = vec2();
    
    for(int i = 0; i < items.size(); ++i) {
        auto a = items[i];
        
        float h = font->measureString(items[i].getName()).y;
        rect.y2 = rect.y1 + h;
        if(p.y > bot)
            break;
        
        if(i == highlightedItem)
            gl::color(highlightColor);
        else
            gl::color(bgColor);
    
        gl::drawSolidRect(rect);
        gl::drawString(a.getName(), p, fontColor);
        
        p.y += h;
        rect.y1 += h;
    }
    
    gl::color(ColorA::white());
}

int ContextMenu::itemOffset(int y)
{
    int cursor = getPos2D().y;
    int i = 0;
    for(auto item : items) {
        int h = font->measureString(item.getName()).y;
        if(cursor > y && (cursor < y + h))
            return i;
        cursor += h;
        ++i;
    }
    return -1;
}

bool ContextMenu::onMouseDown(MouseEvent event)
{
    setVisible(true);
    if(!fixedPos)
        setPos(vec3(event.getPos(), 1000));
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
    game::guiMgr.remove(this);
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

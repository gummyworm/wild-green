//
//  menubar.cpp
//  digital
//
//  Created by Bryce on 8/27/16.
//
//

#include "menubar.hpp"

void MenuBar::addSubmenu(Menu m)
{
    // set the rect of the menu to follow the previous item
    if(menus.size() > 1) {
        Menu *prevMenu = &menus[menus.size()-2];
        m.rect = prevMenu->rect;
        m.rect.x1 += prevMenu->name.length() * 12;
        m.rect.x2 += prevMenu->name.length() * 12;
    } else {
        m.rect = Rectf(0, 0, m.name.length() * 12, height);
    }
    menus.push_back(m);
}

void MenuBar::addItem(string submenu, string name, function<void()> callback)
{
    Menu *sm = nullptr;
    
    for(auto m : menus) {
        if(m.name.compare(submenu) == 0) {
            sm = &m;
            break;
        }
    }
    if(sm != nullptr)
        sm->addItem(name, callback);
}

void MenuBar::draw()
{
    Widget::draw();
    Rectf submenuRect(0, 0, getWindowWidth(), height);
    gl::color(0.8f,0.8f,0.8f);
    gl::drawSolidRect(submenuRect);
    
    for(auto sm : menus) {
        submenuRect.x2 = submenuRect.x1 + sm.name.length() * 12;
        
        Rectf textRect = submenuRect;
        textRect.x1 += 4;
        textRect.y1 = 4;
        
        if(sm.open) {
            gl::color(0, 0, 0);
            gl::drawSolidRect(submenuRect);
            gl::drawString(sm.name, textRect.getUpperLeft(), ColorA(1,1,1));
            sm.draw();
            sm.apply();
        } else {
            gl::color(0.8f,0.8f,0.8f);
            gl::drawSolidRect(submenuRect);
            gl::drawString(sm.name, textRect.getUpperLeft(), ColorA(0,0,0));
        }
        submenuRect.x1 += sm.name.length() * 12;
    }
}

bool MenuBar::onMouseDrag(MouseEvent event)
{
    return false;
}

bool MenuBar::onMouseDown(MouseEvent event)
{
    ivec2 mousePos = event.getPos();

    for(auto &sm : menus) {
        if(sm.contains(mousePos)) {
            sm.open = true;
            return true;
        } else {
            sm.open = false;
        }
    }
    return false;
}

bool MenuBar::onMouseUp(MouseEvent event)
{
    for(auto &sm : menus) {
        if(sm.open == false)
            return true;
    }
    return false;
}
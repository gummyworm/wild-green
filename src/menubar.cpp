//
//  menubar.cpp
//  digital
//
//  Created by Bryce on 8/27/16.
//
//

#include "menubar.hpp"

void MenuBar::addSubmenu(shared_ptr<ContextMenu> m)
{
    m->setPos(m->getPos2D() + ivec2(0, height));
    m->setFixed(true);
    menus.push_back(m);
    font = gl::TextureFont::create(properties::menuFont);
}


void MenuBar::draw()
{
    Widget::draw();
    Rectf submenuRect(0, 0, getWindowWidth(), height);
    gl::color(0.8f,0.8f,0.8f);
    gl::drawSolidRect(submenuRect);
    
    ivec2 cursor = getPos2D();
    for(auto sm : menus) {
        if(sm->isVisible()) {
            gl::color(0.5f, 0.5f, 0.5f);
        } else {
            gl::color(0.8f,0.8f,0.8f);
        }
        float w = font->measureString(sm->getName()).x;
        submenuRect.x2 = cursor.x + w;
        gl::drawSolidRect(submenuRect);
        gl::drawString(sm->getName(), cursor);
        cursor.x += w;
        submenuRect.x1 = submenuRect.x2;
        sm->draw();
        sm->apply();
    }
}

bool MenuBar::onMouseDrag(MouseEvent event)
{
    for(auto sm : menus) {
        if(sm->onMouseDrag(event))
            return true;
    }
    return false;
}

bool MenuBar::onMouseDown(MouseEvent event)
{
    Rectf submenuRect(0, 0, getWindowWidth(), height);
    ivec2 cursor = getPos2D();
    for(auto sm : menus) {
        float w = font->measureString(sm->getName()).x;
        submenuRect.x2 = cursor.x + w;
        if(submenuRect.contains(event.getPos())) {
            sm->setVisible(true);
        }
        cursor.x += w;
        submenuRect.x1 = submenuRect.x2;
        
    }

    for(auto sm : menus) {
        if(sm->isVisible()) {
            if(sm->onMouseDown(event)) {
                return true;
            }
        }
    }
    return false;
}

bool MenuBar::onMouseUp(MouseEvent event)
{
    for(auto sm : menus) {
        sm->onMouseUp(event);
    }
    for(auto sm : menus) {
        sm->setVisible(false);
    }
    return false;
}
//
//  guimanager.cpp
//  digital
//
//  Created by Bryce on 11/2/16.
//
//

#include <algorithm>
#include "guimanager.hpp"

GUIManager::GUIManager()
:widgets()
{
    
}

void GUIManager::addWidget(Widget *w)
{
    newWidgets.push_back(w);
}

void GUIManager::bringFront(Widget *widget)
{
    auto it = find(widgets.begin(), widgets.end(), widget);
    rotate(widgets.begin(), it+1, widgets.end());
}

void GUIManager::moveBack(Widget *widget)
{
    auto it = find(widgets.begin(), widgets.end(), widget);
    rotate(widgets.begin(), it, widgets.end());
}

void GUIManager::update()
{
    for(auto nw : newWidgets)
        widgets.push_back(nw);
    newWidgets.clear();
    
    for(auto w : widgets) {
        w->update();
    }
}

void GUIManager::draw()
{
    for(auto w : widgets) {
        w->draw();
    }
}

void GUIManager::onMouseDown(MouseEvent event)
{
    for(auto w : widgets) {
        w->onMouseDown(event);
    }
}

void GUIManager::onMouseUp(MouseEvent event)
{
    for(auto w : widgets) {
        w->onMouseUp(event);
    }
}

void GUIManager::onMouseMove(MouseEvent event)
{
    for(auto w : widgets) {
        w->onMouseMove(event);
    }
}

void GUIManager::onMouseDrag(MouseEvent event)
{
    for(auto w : widgets) {
        w->onMouseDrag(event);
    }
}

void GUIManager::onKeydown(KeyEvent event)
{
    for(auto w : widgets) {
        w->onKeydown(event);
    }
}

void GUIManager::onAccept(MouseEvent event, shared_ptr<class Entity> e)
{
    for(auto w : widgets) {
        if(w->getRect().contains(event.getPos()))
           w->onAccept(event, e);
    }
}
//
//  guimanager.cpp
//  digital
//
//  Created by Bryce on 11/2/16.
//
//

#include <algorithm>
#include "guimanager.hpp"

bool sortZ(Widget *w1, Widget *w2)
{
    return w1->getPos().z > w2->getPos().z;
}

GUIManager::GUIManager()
:widgets()
{
    
}

void GUIManager::addWidget(Widget *w)
{
    newWidgets.push_back(w);
}

void GUIManager::remove(Widget *w)
{
    toRemove.push_back(w);
}

void GUIManager::bringFront(Widget *widget)
{
    widget->setPos(widget->getPos() + ivec3(0,0,1));
}

void GUIManager::moveBack(Widget *widget)
{
    widget->setPos(widget->getPos() + ivec3(0,0,-1));
}

void GUIManager::update()
{
    if(!toRemove.empty()) {
        vector<Widget*> updated;
        for(auto w : widgets) {
            if(find(toRemove.begin(), toRemove.end(), w) == toRemove.end()) {
                updated.push_back(w);
            } else {
                // TODO: delete
            }
        }
        toRemove.clear();
        widgets = updated;
    }
    
    for(auto nw : newWidgets)
        widgets.push_back(nw);
    newWidgets.clear();
    
    sort(widgets.begin(), widgets.end(), sortZ);
    
    int i = 0;
    for(auto w : widgets) {
        w->update();
        
        ivec3 pos = w->getPos();
        pos.z = i;
        w->setPos(pos);
    }
}

void GUIManager::draw()
{
    reverse(widgets.begin(), widgets.end());
    for(auto w : widgets) {
        w->draw();
        w->apply();
    }
    reverse(widgets.begin(), widgets.end());
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
        if(w->onMouseDrag(event))
            break;
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
        if(w->contains(event.getPos()))
           w->onAccept(event, e);
    }
}
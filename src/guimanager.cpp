//
//  guimanager.cpp
//  digital
//
//  Created by Bryce on 11/2/16.
//
//

#include <algorithm>
#include "guimanager.hpp"

static bool sortZ(shared_ptr<Widget> w1, shared_ptr<Widget> w2)
{
    return w1->getPos().z > w2->getPos().z;
}

GUIManager::GUIManager()
:widgets()
{
    
}

void GUIManager::addWidget(shared_ptr<Widget> w)
{
    newWidgets.push_back(w);
}

void GUIManager::remove(Widget *w)
{
    toRemove.push_back(w);
}

shared_ptr<Widget> GUIManager::getWidget(Widget *widget)
{
    for(auto w : widgets) {
        if(w.get() == widget) {
            return w;
        }
    }
    return nullptr;
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
        vector<shared_ptr<Widget>> updated;
        for(auto w : widgets) {
            if(find(toRemove.begin(), toRemove.end(), w.get()) == toRemove.end()) {
                updated.push_back(w);
            } else {
                // TODO: delete
            }
        }
        widgets = updated;
    }
    
    for(auto nw : newWidgets) {
        widgets.push_back(nw);
    }
    
    newWidgets.clear();
    toRemove.clear();

    sort(widgets.begin(), widgets.end(), sortZ);
    
    for(auto w : widgets) {
        w->update();
    }
}

void GUIManager::draw()
{
    reverse(widgets.begin(), widgets.end());
    for(auto w : widgets) {
        w->draw();
        w->apply();
        gl::disable(GL_SCISSOR_TEST);
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

bool GUIManager::onKeydown(KeyEvent event)
{
    for(auto w : widgets) {
        if(w->onKeydown(event))
            return true;
    }
    return false;
}

bool GUIManager::onAccept(MouseEvent event, shared_ptr<class Entity> e)
{
    for(auto w : widgets) {
        if(w->contains(event.getPos())) {
            if(w->onAccept(event, e)) {
                return true;
            }
        }
    }
    return false;
}

bool GUIManager::onAccept(MouseEvent event, shared_ptr<Widget> widget)
{
    for(auto w : widgets) {
        if(w->contains(event.getPos())) {
            if(w->onAccept(event, widget)) {
                return true;
            }
        }
    }
    return false;
}
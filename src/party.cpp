//
//  party.cpp
//  pool
//
//  Created by Bryce on 8/20/16.
//
//

#include "party.hpp"
#include "gui.h"
#include "properties.h"

Party::Party()
:primaryMember(0)
{
    
}

void Party::draw()
{
    int handStart = gui::PARTY_MEMBER.getWidth()*members.size();
    int handCenter = (properties::screenWidth - handStart) / 2.0f;
    int handStride = (properties::screenWidth - handStart) / members.size();
    int handX = handStart + handCenter - handStride * (members.size()/2.0f);
    
    int y = gui::PARTY.y1;
    int x = gui::PARTY.x1;
    Rectf r(0, 0, gui::PARTY_MEMBER.getWidth(), gui::PARTY_MEMBER.getWidth());
    
    int i = 0;
    for(auto &m : members) {
        if(i == primaryMember) {
            gl::color(1,1,1);
            gl::drawSolidRect((Rectf(vec2(x, y), vec2(x,y)+gui::PARTY_MEMBER.getSize())));
        }
        
        m->setPos(vec2(x, y));
        m->setHandPos(vec2(handX, m->getHandPos().y));
        m->resize(vec2(gui::PARTY_MEMBER.getWidth(), gui::PARTY_MEMBER.getHeight()));
        m->draw();
        
        x += gui::PARTY_MEMBER.getWidth();
        handX += handStride;
        
        ++i;
    }
}

void Party::setPrimary(shared_ptr<PartyMember> member)
{
    int i  = 0;
    for(auto &m : members) {
        if(m.get() == member.get())
            primaryMember = i;
        ++i;
    }
}

void Party::setPrimary(int i)
{
    primaryMember = i;
}

void Party::setPrimaryNext()
{
    if(primaryMember >= members.size()-1)
        setPrimary(0);
    else
        setPrimary(primaryMember+1);
}

void Party::addMember(shared_ptr<PartyMember> m)
{
    m->setPos(vec2(gui::PARTY.x1 + gui::PARTY_MEMBER.getWidth()*members.size(), gui::PARTY.y1));
    m->resize(vec2(gui::PARTY_MEMBER.x2, gui::PARTY_MEMBER.y2));
    members.push_back(m);
}

void Party::onMouseDown(MouseEvent event)
{
    for(auto &m : members) {
        m->onMouseDown(event);
    }
}

void Party::onMouseDrag(MouseEvent event)
{
    for(auto &m : members) {
        m->onMouseDrag(event);
    }
}

void Party::onMouseUp(MouseEvent event)
{
    for(auto &m : members) {
        m->onMouseUp(event);
    }
}
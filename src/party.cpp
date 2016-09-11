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
{
    
}

void Party::draw()
{
    int y = gui::PARTY.y1;
    int x = gui::PARTY.x1;
    Rectf r(0, 0, gui::PARTY_MEMBER.getWidth(), gui::PARTY_MEMBER.getWidth());
    
    for(auto &m : members) {
        m->setPos(vec2(x, y));
        m->resize(vec2(gui::PARTY_MEMBER.getWidth(), gui::PARTY_MEMBER.getHeight()));
        m->draw();
        x += gui::PARTY_MEMBER.getWidth();
    }
}

void Party::addMember(unique_ptr<PartyMember> m)
{
    m->setPos(vec2(gui::PARTY.x1 + gui::PARTY_MEMBER.getWidth()*members.size(), gui::PARTY.y1));
    m->resize(vec2(gui::PARTY_MEMBER.x2, gui::PARTY_MEMBER.y2));
    members.push_back(move(m));
}
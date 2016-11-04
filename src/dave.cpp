//
//  dave.cpp
//  digital
//
//  Created by Bryce on 10/28/16.
//
//

#include "dave.hpp"
#include "npc.hpp"


Dave::Dave()
:NPC("Dave", "person.obj")
{
}

void Dave::onSpeak(Entity *other, string msg)
{
    if(msg.compare("HELLO!") == 0)
        say("HOWDY!!!");
}
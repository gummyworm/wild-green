//
//  door.cpp
//  digital
//
//  Created by Bryce on 9/26/16.
//
//

#include "door.hpp"
#include "game.h"

using namespace std;

Door::Door(bool open, shared_ptr<Entity> key)
:Entity("door"),
open(open),
locked(false),
key(key)
{
    auto fn = bind(&Door::onOpen, this);
    addAction("OPEN", fn);
    
    flags.hasGravity = false;
    flags.grabbable = false;
}

void Door::onAccept(shared_ptr<Entity> e)
{
    if(e.get() == key.get()) {
        open = true;
        //TODO: play open animation
    }
}

SwingingDoor::SwingingDoor(vec3 closedRot, vec3 openRot, float openSpeed, bool open, shared_ptr<Entity> key)
:closedRot(closedRot),
openRot(openRot),
openingTime(0.0f),
openSpeed(openSpeed)
{
    Door::Door(open, key);
}

void Door::update()
{
    Entity::update();
}

void SwingingDoor::onOpen()
{
    if(open)
        return;
    if(locked) {
        say("LOCKED");
        return;
    }
    open = true;
    openingTime = 0.0f;
}

void SwingingDoor::update()
{
    Door::update();
    if(openingTime < openSpeed) {
        openingTime += game::deltaTime;
        rotation = cinder::lerp(closedRot, openRot, openingTime / this->openSpeed);
    }
}

SlidingDoor::SlidingDoor(vec3 closedOffset, vec3 openOffset, float openSpeed, bool open, shared_ptr<Entity> key)
:closedOffset(closedOffset),
openOffset(openOffset),
openingTime(0.0f),
openSpeed(openSpeed)
{
    Door::Door(open, key);
    setPos(closedOffset);
}

void SlidingDoor::onOpen()
{
    if(open)
        return;
    if(locked) {
        say("LOCKED");
        return;
    }
    open = true;
    openingTime = 0.0f;
}

void SlidingDoor::update()
{
    Door::update();
    openingTime += game::deltaTime;
    if(open && (openingTime < openSpeed)) {
        if(openingTime >= this->openSpeed)
            pos = openOffset;
        else
            pos = cinder::lerp(closedOffset, openOffset, openingTime / this->openSpeed);
    }
}

//
//  door.hpp
//  digital
//
//  Created by Bryce on 9/26/16.
//
//

#ifndef door_hpp
#define door_hpp

#include <stdio.h>
#include "entity.hpp"

class Door : public Entity {
protected:
    shared_ptr<Entity> key;
    bool open;
    bool locked;
public:
    Door(bool open = false, shared_ptr<Entity> key=nullptr);
    virtual void onOpen() {open = true;}
    virtual void onClose() {open = false;}
    
    void onAccept(shared_ptr<Entity> e) override;
    void update() override;
};

class SwingingDoor : public Door {
    vec3 closedRot, openRot;
    float openSpeed, openingTime;
public:
    SwingingDoor(vec3 closedRotation=vec3(0,0,0), vec3 openRotation=vec3(0,90,0), float openSpeed=2.0f, bool open = false, shared_ptr<Entity> key=nullptr);
    
    void onOpen() override;
    void update() override;
};

class SlidingDoor : public Door {
    vec3 closedOffset, openOffset;
    float openSpeed, openingTime;
public:
    SlidingDoor(vec3 closedOffset=vec3(0,0,0), vec3 openOffset=vec3(0,1,0), float openSpeed=2.0f, bool open = false, shared_ptr<Entity> key=nullptr);
    
    void onOpen() override;
    void onClose() override;
    void update() override;
};

#endif /* door_hpp */

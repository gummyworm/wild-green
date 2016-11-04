//
//  ai_follow.cpp
//  digital
//
//  Created by Bryce on 10/14/16.
//
//

#include "ai_follow.hpp"
#include "game.h"

AIFollow::AIFollow(Entity *target, float speed)
:target(target),
speed(speed)
{
}

void AIFollow::update()
{
    vec3 from = entity->getPos();
    vec3 to = target->getPos();
    vec3 direction = glm::normalize(to - from);
    vec3 dpos = direction * game::deltaTime * speed;

    //entity->setRot(direction);
    if(glm::distance(to, from) > (game::deltaTime * speed)) {
        entity->setPos(entity->getPos() + dpos);
        return;
    }
    
    entity->setPos(to);
}
//
//  ai_attack.cpp
//  digital
//
//  Created by Bryce on 12/8/16.
//
//

#include "ai_attack.hpp"
#include "game.h"

AIActioner::AIActioner(Entity *target, float actRadius, function<void()> action, float speed)
:target(target),
speed(speed),
attackRadius(attackRadius)
{
    
}

void AIActioner::update()
{
    if(entity == nullptr || target == nullptr)
        return;
    
    vec3 from = entity->getPos();
    vec3 to = target->getPos();
    vec3 direction = glm::normalize(to - from);
    vec3 dpos = direction * game::deltaTime * speed;
    
    //entity->setRot(direction);
    if(glm::distance(to, from) < attackRadius) {
        action();
    } else if(glm::distance(to, from) > (game::deltaTime * speed)) {
        entity->setPos(entity->getPos() + dpos);
        return;
    }
    
    entity->setPos(to);
}

AIAttack::AIAttack(Entity *target, float attackRadius, float speed)
:AIActioner(target, attackRadius, nullptr, speed)
{
}

void AIAttack::attack()
{
    
}
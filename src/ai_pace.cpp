//
//  ai_pace.cpp
//  digital
//
//  Created by Bryce on 10/10/16.
//
//

#include "ai_pace.hpp"
#include "game.h"

AIPace::AIPace(vec3 p1, vec3 p2, float speed)
:speed(speed),
state(MOVE),
rotSpeed(0.5f)
{
    waypoints.push_back(p1);
    waypoints.push_back(p2);
    waypoint = 0;
    dir = 1;
}

void AIPace::update()
{
    vec3 from = entity->getPos();
    vec3 to = waypoints[waypoint + dir];
    vec3 direction = glm::normalize(to - from);
    vec3 dpos = direction * game::deltaTime * speed;

    //entity->setRot(direction);
    if(glm::distance(to, from) > (game::deltaTime * speed)) {
        entity->setPos(entity->getPos() + dpos);
        return;
    }
    
    entity->setPos(to);
    waypoint += dir;
    if(waypoint >= waypoints.size()-1) {
        dir = -1;
        waypoint = waypoints.size()-1;
    } else if(waypoint <= 0) {
        dir = 1;
        waypoint = 0;
    }
}

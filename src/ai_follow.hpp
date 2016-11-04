//
//  ai_follow.hpp
//  digital
//
//  Created by Bryce on 10/14/16.
//
//

#ifndef ai_follow_hpp
#define ai_follow_hpp

#include <stdio.h>
#include "ai_pattern.hpp"

class AIFollow : public AIPattern {
    float speed, rotSpeed;
    Entity *target;
    int waypoint;
    int dir;
public:
    AIFollow(Entity *target, float speed=0.5f);
    void update() override;
};

#endif /* ai_follow_hpp */

//
//  ai_pace.hpp
//  digital
//
//  Created by Bryce on 10/10/16.
//
//

#ifndef ai_pace_hpp
#define ai_pace_hpp

#include <stdio.h>
#include "ai_pattern.hpp"

class AIPace : public AIPattern {
    float speed, rotSpeed;
    enum {
        MOVE,
        PIVOT,
    } state;
    
    vector<vec3> waypoints;
    int waypoint;
    int dir;
public:
    AIPace(vec3 p1, vec3 p2, float speed=0.5f);
    void update() override;
};

#endif /* ai_pace_hpp */

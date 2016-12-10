//
//  trigger.cpp
//  digital
//
//  Created by Bryce on 11/17/16.
//
//

#include "trigger.hpp"
#include "cinder/gl/gl.h"

using namespace ci;

bool PositionTrigger::test(Entity *e)
{
    if(distance(e->getPos(), pos) < radius && !triggered) {
        activate();
        triggered = rearm;
        return true;
    }
    triggered = false;
    return false;
}

void PositionTrigger::draw()
{
    gl::enableWireframe();
    gl::drawSphere(pos, radius);
    gl::disableWireframe();
        
}
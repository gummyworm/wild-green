//
//  game.h
//  digital
//
//  Created by Bryce on 9/6/16.
//
//

#ifndef game_h
#define game_h

#include "playercam.hpp"

extern PlayerCam mainCam;

vec3 screenToWorld(const ivec2 &scr, float z);

#endif /* game_h */

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


namespace game {
    void setup();
    void start();
    void update();
    void draw();
    void resize(ivec2 size);
    
    void onMouseDown(MouseEvent event);
    void onMouseUp(MouseEvent event);
    void onMouseMove(MouseEvent event);
    void onMouseDrag(MouseEvent event);
    void onKeydown(KeyEvent event);
}

#endif /* game_h */

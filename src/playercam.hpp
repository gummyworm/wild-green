//
//  playercam.hpp
//  digital
//
//  Created by Bryce on 8/27/16.
//
//

#ifndef playercam_hpp
#define playercam_hpp

#include <stdio.h>
#include "cinder/Camera.h"
#include "properties.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class PlayerCam : public CameraPersp {
    vec3 viewPos;
    vec3 viewRot;
    vec3 viewDir;
    float moveSpeed;
    float turnSpeed;
    void update();
    
    AxisAlignedBox aabb;
public:
    PlayerCam(float aspectRatio = properties::aspectRatio);
    void onKeyDown(KeyEvent event);
    void onMouseMove(MouseEvent event);
    void onResize();
    
    vec3 getDir() {return viewDir;}
    vec3 getPos() {return viewPos;}
    AxisAlignedBox getAABB() {return aabb;}
    
    void setPos(vec3 p) {viewPos = p; update();}
};


#endif /* playercam_hpp */

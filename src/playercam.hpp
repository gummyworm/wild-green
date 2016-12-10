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
#include "entity.hpp"

using namespace std;
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
    shared_ptr<Entity> entity;
public:
    PlayerCam(float aspectRatio = properties::aspectRatio, shared_ptr<Entity> e=nullptr);
    void onKeyDown(KeyEvent event);
    void onMouseMove(MouseEvent event);
    void onResize();
    
    vec3 getDir() {return viewDir;}
    vec3 getPos() {return viewPos;}
    vec3 getRot() {return viewRot;}
    AxisAlignedBox getAABB() {return aabb;}
    shared_ptr<Entity> getViewer() {return entity;}
    
    void setPos(vec3 p) {viewPos = p; update();}
    void setViewer(shared_ptr<Entity> e) {entity = e; viewPos = entity->getPos();}
};


#endif /* playercam_hpp */

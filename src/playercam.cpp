//
//  playercam.cpp
//  digital
//
//  Created by Bryce on 8/27/16.
//
//

#include "playercam.hpp"
#include "properties.h"

PlayerCam::PlayerCam(float aspectRatio, shared_ptr<Entity> e)
:CameraPersp(),
viewRot(0,0,0),
viewPos(0,.7f,-10),
viewDir(0,0,0),
moveSpeed(0.25f),
turnSpeed(0.1f),
entity(e),
aabb(vec3(-properties::playerAABBSize), vec3(properties::playerAABBSize))
{
    setPerspective(40.0f, aspectRatio, 0, 100.0f);
    update();
}

void PlayerCam::update()
{
    viewDir = vec3(0,0,1);
    viewDir = glm::rotate(viewDir, viewRot.x, vec3(1, 0, 0));
    viewDir = glm::rotate(viewDir, viewRot.y, vec3(0, 1, 0));
    viewDir = glm::rotate(viewDir, viewRot.z, vec3(0, 0, 1));
    lookAt(viewPos, viewPos + viewDir, vec3(0, 1, 0));
    
    aabb = AxisAlignedBox(vec3(viewPos)-vec3(properties::playerAABBSize), vec3(viewPos)+vec3(properties::playerAABBSize));
}

void PlayerCam::onKeyDown(KeyEvent event)
{
    switch(event.getCode()) {
        case KeyEvent::KEY_LEFT:
            viewRot.y += turnSpeed;
            break;
        case KeyEvent::KEY_RIGHT:
            viewRot.y -= turnSpeed;
            break;
        case KeyEvent::KEY_UP:
            viewRot.x -= turnSpeed;
            break;
        case KeyEvent::KEY_DOWN:
            viewRot.x += turnSpeed;
            break;
        case KeyEvent::KEY_w:
            if(event.isShiftDown()) {
                viewPos.y += moveSpeed;
            } else {
                viewPos += viewDir * moveSpeed;
            }
            break;
        case KeyEvent::KEY_s:
            if(event.isShiftDown()) {
                viewPos.y -= moveSpeed;
            } else {
                viewPos -= viewDir * moveSpeed;
            }
            break;
        case KeyEvent::KEY_a:
            viewPos -= glm::cross(viewDir, vec3(0, 1, 0)) * moveSpeed;
            break;
        case KeyEvent::KEY_d:
            viewPos += glm::cross(viewDir, vec3(0, 1, 0)) * moveSpeed;
            break;
        default:
            break;
    }
    if(entity != nullptr) {
        entity->setPos(viewPos);
    }
    update();
}

void PlayerCam::onMouseMove(MouseEvent event)
{
}

void PlayerCam::onResize()
{
    setPerspective(40.0f, getWindowAspectRatio(), 0.01f, 100.0f);
}
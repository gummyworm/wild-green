//
//  playercam.cpp
//  digital
//
//  Created by Bryce on 8/27/16.
//
//

#include "playercam.hpp"

PlayerCam::PlayerCam(float aspectRatio)
:CameraPersp(),
viewRot(0,0,0),
viewPos(0,0,-1),
viewDir(0,0,0),
moveSpeed(0.025f),
turnSpeed(0.02f)
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
}

void PlayerCam::onKeyDown(KeyEvent event)
{
    switch(event.getCode()) {
        case KeyEvent::KEY_UP:
            break;
        case KeyEvent::KEY_DOWN:
            break;
        case KeyEvent::KEY_LEFT:
            viewRot.y += turnSpeed;
            break;
        case KeyEvent::KEY_RIGHT:
            viewRot.y -= turnSpeed;
            break;
        case KeyEvent::KEY_w:
            viewPos += viewDir * moveSpeed;
            break;
        case KeyEvent::KEY_s:
            viewPos -= viewDir * moveSpeed;
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
    update();
}

void PlayerCam::onMouseMove(MouseEvent event)
{
}

void PlayerCam::onResize()
{
    setPerspective(40.0f, getWindowAspectRatio(), 0.01f, 100.0f);
}
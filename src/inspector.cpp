//
//  inspector.cpp
//  digital
//
//  Created by Bryce on 8/26/16.
//
//

#include "inspector.hpp"
#include "cinder/gl/Fbo.h"
#include "properties.h"

Inspector::Inspector(gl::BatchRef batch)
:PRG("inspector"),
batch(batch)
{
    fbo = gl::Fbo::create(512, 512);
    
    cam.lookAt(vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, 0.0f));
    cam.setEyePoint(vec3(0.0f, 0.0f, -10.0f));
    cam.setPerspective(40.0f, getWindowAspectRatio(), 0.01f, 100.0f);
}

void Inspector::draw()
{
    if(!show)
        return;
    
    WidgetWindow::draw();
    
    gl::pushMatrices();
    
    fbo->bindFramebuffer();
    gl::setMatrices(cam);
    gl::clear();
    gl::color(0,1,0);
    gl::setModelMatrix(transform);
    batch->draw();
    fbo->unbindFramebuffer();
    
    gl::setMatricesWindow(properties::screenSize);
    gl::draw(fbo->getColorTexture(), getInternalRect());
    
    gl::color(1,0,1);
    gl::popMatrices();
}

void Inspector::onKeydown(KeyEvent event)
{
    switch(event.getCode()) {
        case KeyEvent::KEY_UP:
            transform = glm::rotate(rotation.y, vec3(1.0f, 0.0f, 0.0f));
            rotation.y -= 0.1f;
            break;
        case KeyEvent::KEY_DOWN:
            transform = glm::rotate(rotation.y, vec3(1.0f, 0.0f, 0.0f));
            rotation.y += 0.1f;
            break;
        case KeyEvent::KEY_LEFT:
            transform = glm::rotate(rotation.x, vec3(0.0f, 1.0f, 0.0f));
            rotation.x -= 0.1f;
            break;
        case KeyEvent::KEY_RIGHT:
            transform = glm::rotate(rotation.x, vec3(0.0f, 1.0f, 0.0f));
            rotation.x += 0.1f;
            break;
        default:
            break;
    }
}
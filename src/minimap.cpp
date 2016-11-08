//
//  minimap.cpp
//  digital
//
//  Created by Bryce on 10/3/16.
//
//

#include "minimap.hpp"
#include "properties.h"
#include "game.h"
#include "gui.h"

MinimapLauncher::MinimapLauncher(shared_ptr<Entity> e)
:PRGLauncher("MAP"),
entity(e)
{
}

void MinimapLauncher::launch()
{
    game::guiMgr.addWidget(new Minimap(entity));
}

bool MinimapLauncher::onMouseDown(MouseEvent event)
{
    return PRGLauncher::onMouseDown(event);
}

Minimap::Minimap(shared_ptr<Entity> e)
:PRG("MAP"),
entity(e)
{
    fbo = gl::Fbo::create(512, 512);
    
    cam.lookAt(vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, 0.0f));
    cam.setEyePoint(vec3(0.0f, 0.0f, -10.0f));
    cam.setPerspective(40.0f, getInternalRect().getWidth() / getInternalRect().getHeight(), 0.01f, 100.0f);
    
    minimapMenu.addItem("EXAMINE", [](){});
    minimapMenu.setVisible(false);
}

void Minimap::draw()
{
    if(!show)
        return;
    WidgetWindow::draw();
    
    gl::pushMatrices();
    
    fbo->bindFramebuffer();
    gl::setMatrices(cam);
    gl::clear(ColorA(1,0,0,1));
    gl::color(0,1,0);
    entity->getBatch()->draw();
    
    gl::setMatricesWindow(properties::screenSize);
    
    // draw the UI: name, health bar, status, etc.
    fbo->unbindFramebuffer();
    
    gl::draw(fbo->getColorTexture(), getInternalRect());
    
    minimapMenu.draw();
    gl::color(1,0,1);
    gl::popMatrices();
}

bool Minimap::onMouseDown(MouseEvent event)
{
    vec3 pt, normal;
    return WidgetWindow::onMouseDown(event);
}

bool Minimap::onMouseDrag(MouseEvent event)
{
    if(WidgetWindow::onMouseDrag(event))
       return true;
    return minimapMenu.onMouseDrag(event);
}

bool Minimap::onMouseUp(MouseEvent event)
{
    minimapMenu.onMouseUp(event);
    return WidgetWindow::onMouseUp(event);
}
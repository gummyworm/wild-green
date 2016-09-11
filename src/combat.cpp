//
//  combat.cpp
//  digital
//
//  Created by Bryce on 9/7/16.
//
//

#include "combat.hpp"
#include "game.h"
#include "gui.h"
#include "properties.h"


CombatLauncher::CombatLauncher(shared_ptr<Entity> e)
:PRGLauncher("FIGHT"),
entity(e)
{
}

void CombatLauncher::launch()
{
    PRGLauncher::launch(shared_ptr<Combat>(new Combat(entity)));
}

void CombatLauncher::onMouseDown(MouseEvent event)
{
    PRGLauncher::onMouseDown(event);
    if(getRect().contains(event.getPos()))
        launch();
}


Combat::Combat(shared_ptr<Entity> e)
:PRG("FIGHT!"),
entity(e)
{
    fbo = gl::Fbo::create(512, 512);
    
    cam.lookAt(vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, 0.0f));
    cam.setEyePoint(vec3(0.0f, 0.0f, -10.0f));
    cam.setPerspective(40.0f, getWindowAspectRatio(), 0.01f, 100.0f);
}

void Combat::draw()
{
    if(!show)
        return;
    WidgetWindow::draw();
    
    gl::pushMatrices();
    
    fbo->bindFramebuffer();
    gl::setMatrices(cam);
    gl::clear(ColorA(1,0,0,1));
    gl::color(0,1,0);
    gl::setModelMatrix(transform);
    entity->getBatch()->draw();
    
    gl::setMatricesWindow(properties::screenSize);
    
    // draw the UI: name, health bar, status, etc.
    Rectf healthBg = gui::COMBAT_HEALTHBAR;
    Rectf healthBar = healthBg;
    healthBar.x2 = gui::COMBAT_HEALTHBAR.x2 * entity->getHP() / entity->getMaxHP();
    
    gl::color(1,1,1,1);
    gl::drawString(entity->getName(), gui::COMBAT_NAME, ColorA(1,1,1,1), gui::COMBAT_FONT);
    gl::color(0,1,1,1);
    gl::drawSolidRect(healthBg);
    gl::color(0,1,0,1);
    gl::drawSolidRect(healthBar);
    
    fbo->unbindFramebuffer();
    gl::draw(fbo->getColorTexture(), getInternalRect());

    gl::color(1,0,1);
    gl::popMatrices();
}

void Combat::onMouseDown()
{
    
}

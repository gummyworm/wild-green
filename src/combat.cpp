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

using namespace std;

CombatLauncher::CombatLauncher(shared_ptr<Entity> e)
:PRGLauncher("FIGHT", "swordicon.png"),
entity(e)
{
}

void CombatLauncher::launch()
{
    game::guiMgr.addWidget(new Combat(entity));
}

void CombatLauncher::onMouseDown(MouseEvent event)
{
    PRGLauncher::onMouseDown(event);
}

void CombatLauncher::onAccept(MouseEvent event, shared_ptr<class Entity> e)
{
    game::guiMgr.addWidget(new Combat(entity));
}

Combat::Combat(shared_ptr<Entity> e)
:PRG("FIGHT!"),
entity(e),
combatMenu()
{
    fbo = gl::Fbo::create(512, 512);
    
    cam.lookAt(vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, 0.0f));
    cam.setEyePoint(vec3(0.0f, 0.0f, -10.0f));
    cam.setPerspective(40.0f, getInternalRect().getWidth() / getInternalRect().getHeight(), 0.01f, 100.0f);
    
    combatMenu.addItem("HIT", [](){});
    combatMenu.addItem("SPELL", [](){});
    combatMenu.addItem("SPEAK", [](){});
    combatMenu.setVisible(false);
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
    
    combatMenu.draw();
    gl::color(1,0,1);
    gl::popMatrices();
}

void Combat::onMouseDown(MouseEvent event)
{
    vec3 pt, normal;
    WidgetWindow::onMouseDown(event);
    if(!game::pick(entity.get(), this, event.getPos() - getInternalPos2D(), &pt, &normal))
        return;
    combatMenu.setPos(event.getPos());
    combatMenu.onMouseDown(event);
}

void Combat::onMouseDrag(MouseEvent event)
{
    WidgetWindow::onMouseDrag(event);
    combatMenu.onMouseDrag(event);
}

void Combat::onMouseUp(MouseEvent event)
{
    combatMenu.onMouseUp(event);
    WidgetWindow::onMouseUp(event);
}
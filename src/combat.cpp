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
    game::guiMgr.addWidget(shared_ptr<Widget>(new Combat(entity)));
}

bool CombatLauncher::onMouseDown(MouseEvent event)
{
    return PRGLauncher::onMouseDown(event);
}

bool CombatLauncher::onAccept(MouseEvent event, shared_ptr<class Entity> e)
{
    game::guiMgr.addWidget(shared_ptr<Widget>(new Combat(entity)));
    return true;
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
    WidgetWindow::draw();
    if(!show)
        return;
    
    gl::pushMatrices();
    
    gl::setMatrices(cam);
    
    fbo->bindFramebuffer();
    gl::pushViewport();
    gl::viewport( fbo->getSize() );
    gl::disable(GL_SCISSOR_TEST);
    gl::clear();
    gl::color(1,1,1);
    entity->getBatch()->draw();
    
    // draw the UI: name, health bar, status, etc.
    gl::setMatricesWindow(properties::screenSize);
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
    
    gl::enable(GL_SCISSOR_TEST);
    gl::popViewport();
    gl::popMatrices();
    gl::draw(fbo->getColorTexture(), getInternalRect());

    //combatMenu.draw();
    //combatMenu.apply();
    gl::color(1,1,1,1);
}

bool Combat::onMouseDown(MouseEvent event)
{
    vec3 pt, normal;
    ivec2 mouseRel = getPos2D() - event.getPos();
    if(WidgetWindow::onMouseDown(event))
        return true;
    
    if(!game::pick(entity.get(), this, event.getPos() - getInternalPos2D(), &pt, &normal))
        return false;
    
    combatMenu.setPos(mouseRel);
    return combatMenu.onMouseDown(event);
}

bool Combat::onMouseDrag(MouseEvent event)
{
    if(WidgetWindow::onMouseDrag(event))
        return true;
    
    return combatMenu.onMouseDrag(event);
}

bool Combat::onMouseUp(MouseEvent event)
{
    if(WidgetWindow::onMouseUp(event))
        return true;
    return combatMenu.onMouseUp(event);
}
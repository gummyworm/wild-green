//
//  entity_widget.cpp
//  digital
//
//  Created by Bryce on 12/5/16.
//
//

#include "entity_widget.hpp"
#include "game.h"

bool EntityWidget::onMouseDown(MouseEvent event)
{
    if(!contains(event.getPos()))
        return false;
    if(event.isLeft()) {
        grab(event.getPos());
    } else if(event.isRight()) {
    }
    return true;
}

bool EntityWidget::onMouseUp(MouseEvent event)
{
    if(grabbed)
        game::guiMgr.onAccept(event, entity);
    if(destroyOnMouseUp)
        game::guiMgr.remove(this);
    
    if(entity->getActions()->isVisible())
        entity->getActions()->onMouseUp(event);
    
    grabbed = false;
    return Widget::onMouseUp(event);
}

bool EntityWidget::onMouseDrag(MouseEvent event)
{
    return Widget::onMouseDrag(event);
}

void EntityWidget::draw()
{
    Widget::draw();
    if(icon != nullptr) {
        if(colorize) {
            float r = entity->getHP() / entity->getMaxHP();
            float g = 1.0f - r;
            gl::color(r, g, 0, 1);
        }
        gl::draw(icon, vec2());;
    }
    gl::color(1,1,1,1);
}
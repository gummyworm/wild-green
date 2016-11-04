//
//  weapon.cpp
//  digital
//
//  Created by Bryce on 10/29/16.
//
//

#include "weapon.hpp"
#include "game.h"

Weapon::Weapon(string name, const fs::path model, int dmg)
:Entity(name, model),
damage(dmg)
{
}

void Weapon::onMouseUp(MouseEvent event)
{
    auto e = game::getPicked(event.getPos());
    if(e != nullptr)
        e->onDamage(this);
}
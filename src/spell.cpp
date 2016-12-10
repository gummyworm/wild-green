//
//  spell.cpp
//  digital
//
//  Created by Bryce on 11/29/16.
//
//

#include "spell.hpp"
#include "game.h"

Spell::Spell(string name)
:name(name)
{
    
}

bool RadiusSpell::onCast(vec3 origin)
{
    for(auto e : game::entitiesInRadius(origin, radius)) {
        onHit(origin, e);
    }
    return true;
}

Projectile::Projectile(const fs::path &model, vec3 dir, float radius, float speed, float lifespan, bool passThrough)
:Entity("", model),
effectRadius(radius),
speed(speed),
dir(dir),
lifespan(lifespan),
passThrough(passThrough)
{
    lifeTimer.start();
}

void Projectile::update()
{
    if(lifeTimer.getSeconds() >= lifespan) {
        setEnabled(false);
        return;
    }
    setPos(getPos() + dir * speed);
    shared_ptr<Entity> e = game::colliding(this);
    if(e != nullptr) {
        for(auto e : game::entitiesInRadius(e->getPos(), effectRadius)) {
            onHit(e);
            if(!passThrough) {
                setEnabled(false);
                return;
            }
        }
    }
}

//
//  zap_spell.cpp
//  digital
//
//  Created by Bryce on 11/29/16.
//
//

#include "zap_spell.hpp"
#include "fx.hpp"
#include "game.h"

Zap::Zap()
:RadiusSpell("ZAP", 100.0f)
{
}

bool Zap::onCast(vec3 origin, shared_ptr<Entity> target)
{
    vec3 dir = normalize(target->getPos() - origin);
    game::add(new Projectile("cube.obj", dir, 0.1f, 5.0f, true));
    fx::flash(ColorA(0,0,0,0), ColorA(0,1,1,1), 0.1f, 3);
    return true;
}

bool Zap::onCast(vec3 origin)
{
    RadiusSpell::onCast(origin);
    fx::flash(ColorA(0,0,0,0), ColorA(0,1,1,1), 0.1f, 3);
    return true;
}

bool Zap::onHit(vec3 origin, shared_ptr<Entity> target)
{
    return true;
}
//
//  spell.hpp
//  digital
//
//  Created by Bryce on 11/29/16.
//
//

#ifndef spell_hpp
#define spell_hpp

#include <stdio.h>
#include "entity.hpp"
#include <cinder/timer.h>

using namespace std;
using namespace ci;
using namespace ci::app;

class Spell {
protected:
    string name;
public:
    Spell(string name = "");
    virtual void cast(vec3 origin, vec3 target) {}
    
    virtual bool onCast(vec3 origin, shared_ptr<Entity> target) {return false;}
    virtual bool onCast(vec3 origin, vec3 target) {return false;}
    virtual bool onCast(vec3 origin) {return false;}
    virtual bool onCast() {return false;}
};

class RadiusSpell : public Spell {
protected:
    float radius;
public:
    RadiusSpell(string name="", float radius=0.0f)
    :Spell(name),
    radius(radius) {
        
    }
    virtual bool onHit(vec3 origin, shared_ptr<Entity> target) {return false;}
    bool onCast(vec3 origin) override;
};

class Projectile : public Entity {
protected:
    float speed;
    float lifespan;
    float effectRadius;

    vec3 dir;
    Timer lifeTimer;
    bool passThrough;
public:
    Projectile(const fs::path &model, vec3 dir, float effectRadius=0.0f, float speed=10.0f, float lifespan=5.0f, bool passThrough=false);
    void update() override;
    virtual void onHit(shared_ptr<Entity> e) {}
};

#endif /* spell_hpp */

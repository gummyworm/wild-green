//
//  ai_attack.hpp
//  digital
//
//  Created by Bryce on 12/8/16.
//
//

#ifndef ai_attack_hpp
#define ai_attack_hpp

#include <functional>
#include <stdio.h>
#include <cinder/Timer.h>
#include "ai_pattern.hpp"

using namespace cinder;
using namespace std;

class AttackAction {
    Entity *entity;
    float range;
    Timer cooldown;
    int damage;
    float cooldownDuration;
public:
    AttackAction(Entity *user, int damage = 10, float range = 1.0f, float cooldown = 1.0f)
    :entity(user),
    range(range),
    damage(damage),
    cooldownDuration(cooldown) {
        this->cooldown.start(cooldown);
    }
    
    virtual void attack(Entity *target) {
        if(glm::distance(entity->getPos(), target->getPos()) < range && cooldown.getSeconds() < cooldownDuration) {
            target->setHP(target->getHP() - damage);
        }
    }
};

class AIActioner : public AIPattern {
    float speed, rotSpeed;
    Entity *target;
    int waypoint;
    int dir;
    float attackRadius;
    
    function<void()> action;
    void attack();
public:
    AIActioner(Entity *target, float actRadius=1.0f, function<void()> action=[](){}, float speed=0.5f);
    void update() override;
    
    void setSpeed(float speed) {this->speed = speed;}
    void setTarget(Entity *target) {target = target;}
};

class AIAttack : public AIActioner {
    float speed, rotSpeed;
    Entity *target;
    int waypoint;
    int dir;
    float attackRadius;
    
    void attack();
public:
    AIAttack(Entity *target, float attackRadius=1.0f, float speed=0.5f);    
    void setSpeed(float speed) {this->speed = speed;}
    void setTarget(Entity *target) {target = target;}
};

#endif /* ai_attack_hpp */

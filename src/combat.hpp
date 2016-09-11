//
//  combat.hpp
//  digital
//
//  Created by Bryce on 9/7/16.
//
//

#ifndef combat_hpp
#define combat_hpp

#include <stdio.h>
#include "prg.hpp"
#include "entity.hpp"

class CombatLauncher : public PRGLauncher {
protected:
    shared_ptr<Entity> entity;
public:
    CombatLauncher(shared_ptr<Entity> e);
    void launch();
    void onMouseDown(MouseEvent event) override;
};

class Combat : public PRG {
    gl::BatchRef batch;
    gl::FboRef fbo;
    CameraPersp cam;

    mat4 transform;
    vec3 rotation;
    shared_ptr<Entity> entity;
public:
    Combat(shared_ptr<Entity> e=nullptr);
    
    void draw() override;
    void onMouseDown();
};

#endif /* combat_hpp */

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
#include "contextmenu.hpp"

class CombatLauncher : public PRGLauncher {
protected:
    shared_ptr<Entity> entity;
public:
    CombatLauncher(shared_ptr<Entity> e);
    void launch() override;
    void onMouseDown(MouseEvent event) override;
    void onAccept(MouseEvent event, shared_ptr<Entity> e) override;
};

class Combat : public PRG {
    gl::BatchRef batch;
    gl::FboRef fbo;
    CameraPersp cam;

    mat4 transform;
    vec3 rotation;
    shared_ptr<Entity> entity;
    
    ContextMenu combatMenu;
public:
    Combat(shared_ptr<Entity> e=nullptr);
    
    void draw() override;
    void onMouseDown(MouseEvent event) override;
    void onMouseDrag(MouseEvent event) override;
    void onMouseUp(MouseEvent event) override;
};

#endif /* combat_hpp */

//
//  minimap.hpp
//  digital
//
//  Created by Bryce on 10/3/16.
//
//

#ifndef minimap_hpp
#define minimap_hpp

#include <stdio.h>
#include "prg.hpp"
#include "entity.hpp"
#include "contextmenu.hpp"

class MinimapLauncher : public PRGLauncher {
protected:
    shared_ptr<Entity> entity;
public:
    MinimapLauncher(shared_ptr<Entity> e);
    void launch() override;
    bool onMouseDown(MouseEvent event) override;
};

class Minimap : public PRG {
    gl::BatchRef batch;
    gl::FboRef fbo;
    CameraPersp cam;
    
    mat4 transform;
    vec3 rotation;
    shared_ptr<Entity> entity;
    
    ContextMenu minimapMenu;
public:
    Minimap(shared_ptr<Entity> e=nullptr);
    
    void draw() override;
    bool onMouseDown(MouseEvent event) override;
    bool onMouseDrag(MouseEvent event) override;
    bool onMouseUp(MouseEvent event) override;
};

#endif /* minimap_hpp */

//
//  entity.hpp
//  pool
//
//  Created by Bryce on 8/21/16.
//
//

#ifndef entity_hpp
#define entity_hpp

#include <string>
#include <stdio.h>
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Batch.h"
#include "cinder/TriMesh.h"
#include "contextmenu.hpp"

using namespace ci;
using namespace std;

enum EntityDrawState {
    DEFAULT,
    SHADED,
    OUTLINE,
};

class Entity {
protected:
    string name;
    vec3 pos, rotation, scale;
    mat4 transform;
    
    TriMesh mesh;
    
    // the velocities that entitites return to the ground at.
    const float FALL_SPEED = 0.0125f;
    
    struct shaders {
        gl::GlslProgRef normal;
        gl::GlslProgRef shaded;
        gl::GlslProgRef outline;
        gl::GlslProgRef ui;
        gl::GlslProgRef shadow;
    }shaders;
    
    struct shadow {
        bool draw;
        ivec2 pos;
        ivec2 grabbedOffset;
        ivec2 dim;
        Rectf texcos;
        gl::FboRef fbo;
    }shadow;
    
    struct flags {
        bool grabbable;
        bool hasGravity;
        bool grounded;
    }flags;
    
    int hp;
    int maxHp;
    
    int memoryUse;
    
    gl::BatchRef batch;
    vector<shared_ptr<Entity>> children;
    ContextMenu actions;
    
    EntityDrawState drawState;
    
    void setupShaders();
    void render(Camera cam);
public:
    Entity(string name="", const fs::path &model="", const fs::path &vert="", const fs::path &frag="");
    void setPos(vec3 pos) {this->pos = pos;}
    void setMemorySize(int sz) {this->memoryUse = sz;}

    void draw(Camera cam);
    void label(Camera cam);
    void update();
    void start() {}
    void addAction(string name, function<void()> fn);
    void setDrawState(EntityDrawState state);
    
    string getName() {return name;}
    TriMesh* getMesh() {return &mesh;}
    gl::BatchRef getBatch() {return batch;}
    mat4 getTransform() {return transform;}
    ContextMenu* getActions() {return &actions;}
    int getHP() {return hp;}
    int getMaxHP() {return maxHp;}
    
    virtual void onMouseDown(MouseEvent event, Camera cam);
    virtual void onMouseDragged(MouseEvent event);
    virtual void onMouseUp(MouseEvent event);
    virtual void onResize();
    
    bool isGrabbed() {return shadow.draw;}
};

#endif /* entity_hpp */

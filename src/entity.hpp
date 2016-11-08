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

struct EntityFlags {
    bool enabled;
    bool grabbable;
    bool movable;
    bool hasGravity;
    bool grounded;
    bool highlight;
    bool solid;
};

struct EntityShadow {
    bool draw;
    ivec2 pos;
    ivec2 grabbedOffset;
    ivec2 dim;
    Rectf texcos;
    gl::FboRef fbo;
};


class Entity {
protected:
    string name;
    vec3 pos, rotation, scale;
    mat4 transform;
    AxisAlignedBox aabb;
    
    TriMeshRef mesh;
    
    // the velocities that entitites return to the ground at.
    const float FALL_SPEED = 0.0125f;
    
    struct shaders {
        gl::GlslProgRef normal;
        gl::GlslProgRef shaded;
        gl::GlslProgRef outline;
        gl::GlslProgRef ui;
        gl::GlslProgRef shadow;
    }shaders;
    
    EntityFlags flags;
    EntityShadow shadow;
    
    int hp;
    int maxHp;
    int memoryUse;
    
    gl::BatchRef batch;
    gl::TextureFontRef font;

    vector<shared_ptr<Entity>> children;
    ContextMenu actions;
    
    EntityDrawState drawState;

    ColorA highlightColor;
    
    void setupShaders();
    void render(Camera cam);
public:
    Entity(string name, const fs::path &model="", gl::GlslProgRef prog=nullptr);
    void setPos(vec3 pos) {this->pos = pos;}
    void setRot(vec3 rot) {this->rotation = rot;}
    void setMemorySize(int sz) {this->memoryUse = sz;}

    virtual void draw(Camera cam);
    virtual void label(Camera cam);
    virtual void update();
    void start() {}
    
    void addAction(string name, function<void()> fn);
    void setDrawState(EntityDrawState state);
    
    void say(string text);
    
    void highlight(ColorA color) {highlightColor = color; flags.highlight = true;}
    void clearHighlight() {flags.highlight = false;}
    
    string getName() {return name;}
    TriMesh* getMesh() {return mesh.get();}
    gl::BatchRef getBatch() {return batch;}
    EntityFlags getFlags() {return flags;}
    
    mat4 getTransform() {return transform;}
    ContextMenu* getActions() {return &actions;}
    int getHP() {return hp;}
    int getMaxHP() {return maxHp;}
    vec3 getPos() {return pos;}
    vec3 getScale() {return scale;}
    vec3 getRotation() {return rotation;}
    
    AxisAlignedBox getAABB() {return aabb;}
    EntityShadow * getShadow() {return &shadow;}
    void setHighlighted(bool highlight) {flags.highlight = highlight;}
    
    virtual void onMouseDown(MouseEvent event, Camera cam);
    virtual void onMouseDragged(MouseEvent event);
    virtual void onMouseUp(MouseEvent event);
    virtual void onMouseMove(MouseEvent event);
    virtual void onResize();
    
    // called when another entity is is dropped on this entity.
    virtual void onAccept(shared_ptr<Entity> e) {}
    
    // called when another entity engages this entity in conversation.
    virtual void onSpeak(Entity *speaker, string msg) {}
    
    // called when another entity damages this entity.
    virtual void onDamage(Entity *dealer, int amount=0) {}
    
    bool isGrabbed() {return shadow.draw;}
    void setShadow(bool on) {shadow.draw = on;}
    bool isEnabled() {return flags.enabled;}
    void setEnabled(bool enable) {flags.enabled = enable;}
    void generateShadow();
    void setShadowPos(ivec2 pos) {shadow.pos = pos;}
};

#endif /* entity_hpp */

//
//  game.h
//  digital
//
//  Created by Bryce on 9/6/16.
//
//

#ifndef game_h
#define game_h

#include "cinder/gl/gl.h"
#include "entity.hpp"
#include "playercam.hpp"
#include "speechbubble.hpp"
#include "window.hpp"
#include "guimanager.hpp"
#include "speech.hpp"

extern PlayerCam mainCam;

vec3 screenToWorld(const ivec2 &scr, float z);

namespace game {
    void add(Entity *e, vec3 pos=vec3());
    void remove(Entity *e);
    
    void setup();
    void start();
    void update();
    void draw();
    void resize(ivec2 size);
    
    void onMouseDown(MouseEvent event);
    void onMouseUp(MouseEvent event);
    void onMouseMove(MouseEvent event);
    void onMouseDrag(MouseEvent event);
    void onKeydown(KeyEvent event);
    shared_ptr<Entity> colliding(Entity *e);

    bool pick(Entity *e, WidgetWindow *win, vec2 pos, vec3 *pickedPoint, vec3 *pickedNormal);
    vector<shared_ptr<Entity>> entitiesInRadius(vec3 origin, float radius);

    shared_ptr<Entity> getPicked(ivec2 pos);
    vec3 screenToWorld(ivec2 pos, float distance);
    
    extern gl::GlslProgRef uiProg;
    extern float deltaTime;
    
    void say(Entity *speaker, string msg);
    void grab(Entity *e);

    void setClearColor(ColorA color);
    
    extern SpeechManager speechMgr;
    extern GUIManager guiMgr;
}

#endif /* game_h */

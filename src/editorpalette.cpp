//
//  editorpalette.cpp
//  digital
//
//  Created by Bryce on 11/10/16.
//
//

#include "editorpalette.hpp"
#include "game.h"

EditorPaletteLauncher::EditorPaletteLauncher()
:PRGLauncher("FIGHT", "swordicon.png")
{
    
}

vec3 EditorPalette::getPlacePos()
{
    return vec3(ivec3(mainCam.getEyePoint()) + ivec3(mainCam.getViewDirection() * 5.0f)) + placeOffset;
}

void EditorPalette::paint(function<Entity*()> spawn)
{
    if(brushSize.x < 1)
        brushSize.x = 1;
    if(brushSize.y < 1)
        brushSize.y = 1;
    if(brushSize.z < 1)
        brushSize.z = 1;
    
    for(int i = 0; i < brushSize.x; ++i) {
        for(int j = 0; j < brushSize.y; ++j) {
            for(int k = 0; k < brushSize.z; ++k) {
                Entity *e = spawn();
                cursor = e->getBatch();
                game::add(spawn(), (getPlacePos() + vec3(i, j, k)));
            }
        }
    }
}


EditorPalette::EditorPalette()
:PRG("EDIT"),
brushSize(1, 1, 1),
cursorAdvance(-1,0,0)
{
    int w = 2;
    int h = 5;
    
    cursor = gl::Batch::create(geom::Cube(), gl::getStockShader(gl::ShaderDef().lambert().color()));
    
    spawners = {
        [this]() {paint([](){
            return new Entity("cube", "cube.obj");
        });},
        [this]() {paint([](){
            return new Entity("cactus", "cactus1.obj");
        });},
        [this]() {paint([](){
            return new Entity("teapot", "teapot.obj");
        });},
        [this]() {game::add(new Entity("A"), getPlacePos()); cout << "HI" << endl;},
        [this]() {game::add(new Entity("A"), getPlacePos()); cout << "HI" << endl;},
        [this]() {game::add(new Entity("A"), getPlacePos()); cout << "HI" << endl;},
        [this]() {game::add(new Entity("A"), getPlacePos()); cout << "HI" << endl;},
        [this]() {game::add(new Entity("A"), getPlacePos()); cout << "HI" << endl;},
        [this]() {game::add(new Entity("A"), getPlacePos()); cout << "HI" << endl;},
        [this]() {game::add(new Entity("A"), getPlacePos()); cout << "HI" << endl;},
        [this]() {game::add(new Entity("A"), getPlacePos()); cout << "HI" << endl;},
        [this]() {game::add(new Entity("A"), getPlacePos()); cout << "HI" << endl;}
    };
    
    grid = new ButtonGrid(this, w, h);
    for(int i = 0; i < w*h; ++i) {
        new Button(grid, spawners[i]);
    }
    grid->setPos(ivec3(0,16,0));
    grid->layout();
    
    resize(grid->getDim() + ivec2(100,160));
    placeOffset = vec3(0,0,0);
}

void EditorPalette::draw()
{
    PRG::draw();
    gl::setMatrices(mainCam);
    gl::translate(getPlacePos());
    
    gl::disable(GL_SCISSOR_TEST);
    gl::enableWireframe();
    
    gl::translate(-1,-1,-1);
    for(int i = 0; i < brushSize.x; ++i) {
        gl::translate(1, 0, 0);
        gl::pushMatrices();
        for(int j = 0; j < brushSize.y; ++j) {
            gl::translate(0, 1, 0);
            gl::pushMatrices();
            for(int k = 0; k < brushSize.z; ++k) {
                gl::translate(0, 0, 1);
                cursor->draw();
            }
            gl::popMatrices();
        }
        gl::popMatrices();
    }
    gl::disableWireframe();
}

bool EditorPalette::onKeydown(KeyEvent event)
{
    if(!event.isAltDown())
        return false;
    
    switch (event.getCode()) {
        case KeyEvent::KEY_UP:
            cursorAdvance = vec3(0,0,1);
            if(event.isShiftDown())
                brushSize += cursorAdvance;
            else
                placeOffset += cursorAdvance;
            return true;;
        case KeyEvent::KEY_DOWN:
            cursorAdvance = vec3(0,0,-1);
            if(event.isShiftDown())
                brushSize += cursorAdvance;
            else
                placeOffset += cursorAdvance;
            return true;
        case KeyEvent::KEY_LEFT:
            cursorAdvance = vec3(1,0,0);
            if(event.isShiftDown())
                brushSize += cursorAdvance;
            else
                placeOffset += cursorAdvance;
            return true;
        case KeyEvent::KEY_RIGHT:
            cursorAdvance = vec3(-1,0,0);
            if(event.isShiftDown())
                brushSize += cursorAdvance;
            else
                placeOffset += cursorAdvance;
            return true;
        case KeyEvent::KEY_q:
            spawners[grid->getSelectedIndex()]();
            placeOffset += cursorAdvance;
            return true;
        case KeyEvent::KEY_EQUALS:
            
        default:
            return true;
    }
    return false;
}
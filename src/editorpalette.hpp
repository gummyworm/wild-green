//
//  editorpalette.hpp
//  digital
//
//  Created by Bryce on 11/10/16.
//
//

#ifndef editorpalette_hpp
#define editorpalette_hpp

#include <stdio.h>
#include "prg.hpp"
#include "cinder/gl/gl.h"
#include "cinder/gl/texture.h"
#include "ButtonGrid.hpp"

using namespace ci::app;
using namespace std;
using namespace ci;

class EditorPaletteLauncher : public PRGLauncher {
protected:
    shared_ptr<Entity> entity;
public:
    EditorPaletteLauncher();
    void launch() override;
    bool onMouseDown(MouseEvent event) override;
    bool onAccept(MouseEvent event, shared_ptr<Entity> e) override;
};

class EditorPalette : public PRG {
    ivec3 brushSize;
protected:
    vec3 placeOffset;
    vec3 cursorAdvance;
    vec3 getPlacePos();
    ButtonGrid *grid;
    vector<function<void()>> spawners;
    function<void()> quickAction;
    gl::BatchRef cursor;
public:
    EditorPalette();
    void draw() override;
    
    void paint(function<Entity*()> spawn);
    bool onKeydown(KeyEvent event) override;
};
#endif /* editorpalette_hpp */

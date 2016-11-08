//
//  prg.hpp
//  digital
//
//  Created by Bryce on 8/26/16.
//
//

#ifndef prg_hpp
#define prg_hpp

#include <stdio.h>
#include "window.hpp"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Batch.h"

using namespace ci;
using namespace cinder;
using namespace ci::app;

class PRG : public WidgetWindow{
protected:
public:
    PRG(string name="", string icon="", int w=240, int h=160):WidgetWindow(name, w, h) {}
};

class PRGLauncher : public Widget {
protected:
    struct Icon {
        gl::TextureRef tex;
    } icon;
    
    string name;
    vector<shared_ptr<PRG>> instances;
    
    Timer doubleClickTimer;
    bool grabbed;
    ivec2 grabOffset;

public:
    PRGLauncher(string name, string icon="");
    void draw() override;
    virtual void launch() {}
    bool onMouseDown(MouseEvent event) override;
    bool onMouseUp(MouseEvent event) override;
    bool onMouseDrag(MouseEvent event) override;
    bool onAccept(MouseEvent event, shared_ptr<class Entity> e) override;
};

#endif /* prg_hpp */

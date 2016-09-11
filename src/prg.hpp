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
    PRG(string name="", string icon=""):WidgetWindow(name) {}
};

class PRGLauncher : public Widget {
protected:
    struct Icon {
        gl::TextureRef tex;
    } icon;
    
    string name;
    vector<shared_ptr<PRG>> instances;

public:
    PRGLauncher(string name, string icon="");
    void draw() override;
    virtual void launch(shared_ptr<PRG> instance) {instances.push_back(instance);}
    void onMouseDown(MouseEvent event) override;
    void onMouseDrag(MouseEvent event) override;
    void onAccept(MouseEvent event, shared_ptr<class Entity> e) override;
};

#endif /* prg_hpp */

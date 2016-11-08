//
//  inspector.hpp
//  digital
//
//  Created by Bryce on 8/26/16.
//
//

#ifndef inspector_hpp
#define inspector_hpp

#include <stdio.h>
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Batch.h"
#include "prg.hpp"

using namespace ci;
using namespace cinder;
using namespace ci::app;

class Inspector : public PRG {
protected:
    gl::BatchRef batch;
    gl::FboRef fbo;
    CameraPersp cam;
    
    mat4 transform;
    vec3 rotation;
public:
    Inspector(gl::BatchRef batch);
    
    void setBatch(gl::Batch batch) {batch = batch;}
    void draw() override;
    void resize(ivec2 dim) override;
    bool onKeydown(KeyEvent event) override;
};

#endif /* inspector_hpp */

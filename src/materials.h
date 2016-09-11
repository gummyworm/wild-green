//
//  materials.h
//  pool
//
//  Created by Bryce on 8/20/16.
//
//

#ifndef materials_h
#define materials_h

#include "post.hpp"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"

using namespace ci;

class Materials {
public:
    gl::GlslProgRef postPass;
    void setup();
};

#endif /* materials_h */

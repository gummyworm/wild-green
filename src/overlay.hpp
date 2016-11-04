//
//  overlay.hpp
//  digital
//
//  Created by Bryce on 9/22/16.
//
//

#ifndef overlay_hpp
#define overlay_hpp

#include <string>
#include <stdio.h>
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace std;

class Overlay {
protected:
    gl::TextureRef tex;
    Rectf rect;
public:
    Overlay(const fs::path image="", Rectf rect=Rectf());
    void setImage(const fs::path image);
    void draw();
    void setPos(ivec2 pos);
    
    ivec2 getPos() {return rect.getUpperLeft();}
};

#endif /* overlay_hpp */

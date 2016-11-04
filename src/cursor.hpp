//
//  cursor.hpp
//  digital
//
//  Created by Bryce on 9/22/16.
//
//

#ifndef cursor_hpp
#define cursor_hpp

#include <stdio.h>
#include <string>
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Cursor {
protected:
    ivec2 pos;
    gl::TextureRef tex;
    Rectf rect;
public:
    Cursor();
    Cursor(const fs::path image);
    void draw();
    
    ivec2 getPos() {return pos;}
    void onMouseMove(MouseEvent event);
};

#endif /* cursor_hpp */

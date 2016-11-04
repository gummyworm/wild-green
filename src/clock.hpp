//
//  clock.hpp
//  digital
//
//  Created by Bryce on 9/27/16.
//
//

#ifndef clock_hpp
#define clock_hpp

#include <stdio.h>

#include "entity.hpp"

class Clock : public Entity {
    gl::FboRef timeFbo;
    gl::TextureFontRef font;

public:
    Clock();
    
    void setMessage(string msg);
    void draw();
};

#endif /* clock_hpp */

//
//  fx.hpp
//  digital
//
//  Created by Bryce on 12/1/16.
//
//

#ifndef fx_hpp
#define fx_hpp

#include <stdio.h>
#include <cinder/gl/gl.h>

using namespace std;
using namespace ci;
using namespace cinder;

namespace fx {
    void flash(ColorA color1, ColorA color2, float duration, int repeats=0);

    void update();
}

#endif /* fx_hpp */

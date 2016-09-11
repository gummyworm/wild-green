//
//  properties.h
//  digital
//
//  Created by Bryce on 9/2/16.
//
//

#ifndef properties_h
#define properties_h

#include "cinder/gl/gl.h"
using namespace ci;
using namespace std;

namespace properties {
    const int screenWidth = 640;
    const int screenHeight = 480;
    const ivec2 screenSize(screenWidth, screenHeight);
    const float aspectRatio = ((float)screenWidth) / screenHeight;
    
    const Font font = Font("Arial", 22);

    const ivec2 speechboxSize = ivec2(300, 120);
    const ColorA speechboxBgColor = ColorA(0,0,0,1);
    const ColorA speechboxTextColor = ColorA(1,1,1,1);
}

#endif /* properties_h */

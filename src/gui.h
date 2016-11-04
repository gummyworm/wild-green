//
//  gui.h
//  pool
//
//  Created by Bryce on 8/21/16.
//
//

#ifndef gui_h
#define gui_h

#include "cinder/gl/gl.h"
#include "properties.h"

namespace gui {
    const Rectf PARTY(0, properties::screenHeight-64, properties::screenWidth, properties::screenHeight);
    const Rectf PARTY_MEMBER(0, 0, 32, 64);
    
    const float windowBorderWidth = 12;
    const ColorA windowBorderColor(0.5, 0.5, 0.5, 1);
    const ColorA windowBgColor(1, 1, 1, 1);
    const Rectf windowResizeHeatRect(-windowBorderWidth-8, -windowBorderWidth-8, 0, 0); // relative to lower-right
    
    const vec2 COMBAT_NAME(0,0);
    const Rectf COMBAT_HEALTHBAR(0, 48, 100, 58);
    const Font COMBAT_FONT("Arial", 48);
}

#endif /* gui_h */

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
    const Rectf PARTY(0, properties::screenHeight-60, properties::screenWidth, 480);
    const Rectf PARTY_MEMBER(0, 0, 60, 60);

    const vec2 COMBAT_NAME(0,0);
    const Rectf COMBAT_HEALTHBAR(0, 48, 100, 58);
    const Font COMBAT_FONT("Arial", 48);
}

#endif /* gui_h */

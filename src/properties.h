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
#include "cinder/app/KeyEvent.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace properties {
    const int screenWidth = 640;
    const int screenHeight = 480;
    const ivec2 screenSize(screenWidth, screenHeight);
    const float aspectRatio = ((float)screenWidth) / screenHeight;
    
    const float playerAABBSize = 0.1f;
    
    const ivec2 cursorSize = ivec2(32, 32);
    
    
    // colors
    const ColorA highlightColor = ColorA(0, 0, 1, 1);
    const ColorA labelColor = ColorA(0.5f, 0.5f, 0.5f, 1);
    const ColorA labelTextColor = ColorA(1, 1, 1, 1);
    const ColorA iconGrabbedColor = ColorA(0.5f, 0.5f, 0.5f, 1);
    const ColorA moveHighlightColor = ColorA(0.5f, 0.5f, 0.5f, 1);

    // player properties
    const Rectf portraitRect = Rectf(0,0,32,64);
    const ivec2 speechboxSize = ivec2(300, 120);
    const ivec2 speechboxOffset = ivec2(10, 55);
    const ivec2 speechboxTextOffset = ivec2(5,16);
    const ColorA speechboxBgColor = ColorA(1,1,1,0.75f);
    const float speechboxBorderWidth = 4.0f;
    const ColorA speechboxBorderColor = ColorA(0,1,0,1);
    const ColorA speechboxTextColor = ColorA(1,1,1,1);
    const Rectf playerHandRect = Rectf(screenWidth/2, screenHeight-64, screenWidth/2+64, screenHeight-64+64);
    const float handBobAmplitude = 10.0f;
    
    // Fonts
    const Font speechboxFont = Font("Arial", 16);
    const Font font = Font("Arial", 22);
    const Font clockFont = Font("Arial", 20);
    const Font entityLabelFont = Font("Arial", 26);
    
    const float doubleClickDelay = 1.5f;
    
    // Keybindings
    const key_t KEY_NEXT_PARTY_MEMBER = KeyEvent::KEY_TAB;
}

#endif /* properties_h */

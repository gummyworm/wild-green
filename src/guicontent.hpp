//
//  guicontent.hpp
//  digital
//
//  Created by Bryce on 11/10/16.
//
//

#ifndef guicontent_hpp
#define guicontent_hpp

#include <stdio.h>
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class GUIContent {
    gl::TextureRef texture;
    string text;
public:
    GUIContent(string text):text(text) {}
};

#endif /* guicontent_hpp */

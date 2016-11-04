//
//  TexturedCubeTile.hpp
//  digital
//
//  Created by Bryce on 10/24/16.
//
//

#ifndef TexturedCubeTile_hpp
#define TexturedCubeTile_hpp

#include <stdio.h>
#include "worldmap.hpp"

class TexturedCubeTile : public MapTile {
protected:
    gl::TextureRef tex;
public:
    TexturedCubeTile(const fs::path texture);
    void draw() override;
};

#endif /* TexturedCubeTile_hpp */

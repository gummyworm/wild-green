//
//  tiles.hpp
//  digital
//
//  Created by Bryce on 10/30/16.
//
//

#ifndef tiles_hpp
#define tiles_hpp

#include <stdio.h>
#include "worldmap.hpp"

class GrassTile : public MapTile {
public:
    GrassTile():MapTile("grass.obj", false) {}
};

class TreeTile : public MapTile {
public:
    TreeTile():MapTile("tree.obj") {}
};

#endif /* tiles_hpp */

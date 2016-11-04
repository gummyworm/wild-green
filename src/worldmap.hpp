//
//  worldmap.hpp
//  digital
//
//  Created by Bryce on 9/11/16.
//
//

#ifndef worldmap_hpp
#define worldmap_hpp

#include <stdio.h>
#include <vector>
#include "cinder/gl/gl.h"
#include "cinder/gl/Batch.h"

using namespace std;
using namespace cinder;

class MapTile {
protected:
    gl::BatchRef batch;
    
    bool solid;
public:
    MapTile(gl::BatchRef batch = nullptr, bool solid=true);
    MapTile(const fs::path, bool solid=true);
    virtual void draw();
    
    bool isSolid() {return solid;}
};

class WorldMap {
    const vec3 tileSize = vec3(1,1,1);
    
    map<int, MapTile*> idsToTiles;
    vector<vector<vector<MapTile*>>> tiles;

    gl::BatchRef floor;
    
public:
    WorldMap();
    WorldMap(map<int, MapTile> idsToTiles, vector<vector<vector<int>>> tiles);
    
    void setTile(int x, int y, int z, int id);
    void registerId(int id, MapTile *tile) {idsToTiles[id] = tile;}
    
    void draw();
    void load(vector<vector<vector<int>>> tiles);
    bool intersects(AxisAlignedBox aabb);
    bool containsPoint(vec3 pt);
};

#endif /* worldmap_hpp */

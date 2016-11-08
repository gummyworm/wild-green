//
//  worldmap.cpp
//  digital
//
//  Created by Bryce on 9/11/16.
//
//

#include "worldmap.hpp"
#include "game.h"
#include "TexturedCubeTile.hpp"
#include "cinder/ObjLoader.h"
#include "tiles.hpp"

WorldMap::WorldMap()
:floor(nullptr)
{
}

WorldMap::WorldMap(map<int, MapTile> idsToTiles, vector<vector<vector<int>>> tiles)
:WorldMap()
{
    idsToTiles = idsToTiles;
    tiles = tiles;
}

void WorldMap::draw()
{
    gl::pushModelMatrix();
    
    // draw the floor
    if(floor != nullptr) {
        gl::pushMatrices();
        gl::translate(0,-0.5,0);
        gl::scale(100,100,100);
        floor->draw();
        gl::popMatrices();
    } else {
        auto shader = gl::GlslProg::create(loadAsset("solidcolor.vert"), loadAsset("solidcolor.frag"));
        floor = gl::Batch::create(geom::Plane(), shader); //gl::getStockShader(gl::ShaderDef().lambert().color()));
    }
    //game::uiProg->bind();
    //gl::color(0,1,0,1);
    //gl::drawCube(vec3(0,-300,0), vec3(600));
    //gl::color(1, 0, 0);
    
    int x, y, z;
    
    z = 0;
    for(auto layer : tiles) {
        y = 0;
        for(auto row : layer) {
            x = 0;
            for(auto tile : row) {
                gl::setMatrices(mainCam);
                mat4 transform = mat4() * glm::translate(vec3(tileSize.x * x, tileSize.z * z - tileSize.z, tileSize.y * y));
                gl::setModelMatrix(transform);
                tile->draw();
                x++;
            }
            y++;
        }
        z++;
    }
    gl::popModelMatrix();
}

void WorldMap::setTile(int x, int y, int z, int id)
{
    MapTile *tile = nullptr;
    switch(id) {
        case 0:
            tile = new MapTile((gl::BatchRef)nullptr, false);
            break;
        case 1:
            tile = new TexturedCubeTile("checkerboard.jpg");
            break;
        case 2:
            tile = new TreeTile();
            break;
        case 3:
            tile = new GrassTile();
            break;
        default:
            tile = new MapTile();
            break;
    }
    
    tiles[z][y][x] = tile;
}

void WorldMap::load(vector<vector<vector<int> >> ids)
{
    int z = 0;
    tiles = vector<vector<vector<MapTile*>>>();
    
    for(auto layer : ids) {
        tiles.push_back(vector<vector<MapTile*>>());
        int y = 0;
        for(auto row : layer) {
            int x = 0;
            tiles[z].push_back(vector<MapTile*>());
            tiles[z][y].resize(row.size());
            for(auto id : row) {
                setTile(x, y, z, id);
                x++;
            }
            y++;
        }
        z++;
    }
}

bool WorldMap::intersects(AxisAlignedBox aabb)
{
    int z = 0;
    for(auto layer : tiles) {
        int y = 0;
        for(auto row : layer) {
            int x = 0;
            for(auto tile : row) {
                AxisAlignedBox tileBox = AxisAlignedBox(vec3(x,y,z)+vec3(0.5f), vec3(x,y,z)-vec3(0.5f));
                if(tile->isSolid() && aabb.intersects(tileBox))
                   return true;
                x++;
            }
            y++;
        }
        z++;
    }
    return false;
}

bool WorldMap::containsPoint(vec3 pt)
{
    int z = 0;
    for(auto layer : tiles) {
        int y = 0;
        for(auto row : layer) {
            int x = 0;
            for(auto tile : row) {
                AxisAlignedBox tileBox = AxisAlignedBox(vec3(x,z,y)-vec3(0.5f), vec3(x,z,y)+vec3(0.5f));
                if(tile->isSolid() && tileBox.contains(pt))
                    return true;
                x++;
            }
            y++;
        }
        z++;
    }
    return false;
}


MapTile::MapTile(gl::BatchRef batch, bool solid)
:batch(batch),
solid(solid)
{
}

void MapTile::draw()
{
    if(batch != nullptr)
        batch->draw();
}

MapTile::MapTile(const fs::path filename, bool solid)
:solid(solid)
{
    ObjLoader ol(loadAsset(filename));
    batch = gl::Batch::create(ol, gl::getStockShader( gl::ShaderDef().lambert().color() ) );
}


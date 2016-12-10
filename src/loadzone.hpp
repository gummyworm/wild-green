//
//  loadzone.hpp
//  digital
//
//  Created by Bryce on 11/9/16.
//
//

#ifndef loadzone_hpp
#define loadzone_hpp

#include <stdio.h>
#include "entity.hpp"
#include "worldmap.hpp"

class LoadZone : public Entity {
protected:
    shared_ptr<WorldMap> endpoint1, endpoint2;
public:
    LoadZone(string name, shared_ptr<WorldMap> map1, shared_ptr<WorldMap> map2):Entity(name), endpoint1(map1), endpoint2(map2) {}
    virtual void onEnter(shared_ptr<Entity> e);
};

#endif /* loadzone_hpp */

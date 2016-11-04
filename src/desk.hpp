//
//  desk.hpp
//  digital
//
//  Created by Bryce on 9/26/16.
//
//

#ifndef desk_hpp
#define desk_hpp

#include <stdio.h>
#include "entity.hpp"
#include "overlay.hpp"
#include "properties.h"

class Desk : public Overlay {
protected:
    vector<Entity> items;
public:
    Desk(const fs::path image="", Rectf rect=Rectf(vec2(), properties::screenSize));
};

#endif /* desk_hpp */

//
//  ai_pattern.hpp
//  digital
//
//  Created by Bryce on 10/10/16.
//
//

#ifndef ai_pattern_hpp
#define ai_pattern_hpp

#include <stdio.h>
#include "entity.hpp"

class AIPattern {
protected:
    Entity *entity;
public:
    AIPattern(Entity *entity=nullptr);
    virtual void update() {}
    void possess(Entity *entity) {this->entity = entity;}
};

#endif /* ai_pattern_hpp */

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
#include <cinder/gl/gl.h>
#include "entity.hpp"

class AIPattern {
protected:
    Entity *entity;
    gl::TextureRef icon;
public:
    AIPattern(Entity *entity=nullptr, const fs::path icon="");
    virtual void update() {}
    void possess(Entity *entity) {this->entity = entity;}
    
    gl::TextureRef getIcon() {return icon;}
};

#endif /* ai_pattern_hpp */

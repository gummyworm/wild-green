//
//  zap_spell.hpp
//  digital
//
//  Created by Bryce on 11/29/16.
//
//

#ifndef zap_spell_hpp
#define zap_spell_hpp

#include <stdio.h>
#include "spell.hpp"
#include "entity.hpp"

class Zap : public RadiusSpell {
public:
    Zap();
    bool onCast(vec3 origin) override;
    bool onCast(vec3 origin, shared_ptr<Entity> target) override;
    bool onHit(vec3 origin, shared_ptr<Entity> target) override;
};

#endif /* zap_spell_hpp */

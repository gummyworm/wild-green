//
//  dave.hpp
//  digital
//
//  Created by Bryce on 10/28/16.
//
//

#ifndef dave_hpp
#define dave_hpp

#include <stdio.h>
#include "npc.hpp"

class Dave : public NPC {
public:
    Dave();
    void onSpeak(Entity *other, string msg) override;
};

#endif /* dave_hpp */

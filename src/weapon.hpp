//
//  weapon.hpp
//  digital
//
//  Created by Bryce on 10/29/16.
//
//

#ifndef weapon_hpp
#define weapon_hpp

#include <stdio.h>
#include "entity.hpp"

class Weapon : public Entity {
protected:
    int damage;
    
public:
    Weapon(string name="", const fs::path model="", int dmg=0);
    void onMouseUp(MouseEvent event) override;
};

#endif /* weapon_hpp */

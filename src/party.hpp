//
//  party.hpp
//  pool
//
//  Created by Bryce on 8/20/16.
//
//

#ifndef party_hpp
#define party_hpp

#include <stdio.h>
#include "partymember.hpp"

using namespace std;

class Party {
    vector<unique_ptr<PartyMember>> members;
public:
    Party();
    void draw();
    void update() {}
    
    void addMember(unique_ptr<PartyMember> m);
};

#endif /* party_hpp */

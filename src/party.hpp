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
    int primaryMember;
    
public:
    vector<shared_ptr<PartyMember>> members;
    
    Party();
    void draw();
    void update() {}
    void setPrimary(shared_ptr<PartyMember> member);
    void setPrimary(int i);
    void setPrimaryNext();
    
    void addMember(shared_ptr<PartyMember> m);
    void onMouseDown(MouseEvent event);
    void onMouseUp(MouseEvent event);
    void onMouseDrag(MouseEvent event);
    
    int getPrimary() {return primaryMember;}
};

#endif /* party_hpp */

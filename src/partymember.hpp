//
//  partymember.hpp
//  pool
//
//  Created by Bryce on 8/20/16.
//
//

#ifndef partymember_hpp
#define partymember_hpp

#include <string>
#include <stdio.h>
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "widget.hpp"
#include "speechbubble.hpp"
#include "conversation.hpp"

using namespace ci;
using namespace std;

class PartyMember : public Widget {
    string name;
    gl::TextureRef portrait;
    SpeechBubble speech;
    
public:
    PartyMember(string name, const fs::path portrait);
    
    void draw();
    void joinConversation(Conversation *convo);
    void say(string msg);
};

#endif /* partymember_hpp */

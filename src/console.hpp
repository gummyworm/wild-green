//
//  console.hpp
//  digital
//
//  Created by Bryce on 11/21/16.
//
//

#ifndef console_hpp
#define console_hpp

#include <stdio.h>
#include "textedit.hpp"
#include "cinder/Timer.h"

class Console : public TextLog {
protected:
    string command;
    Timer blinkTimer;
    
    ivec2 cursor;
public:
    Console();

    bool onKeydown(KeyEvent event) override;
    bool onAccept(MouseEvent event, shared_ptr<class Entity> e) override;
    void draw() override;
    void execute();
    void append(string text);
};

#endif /* console_hpp */

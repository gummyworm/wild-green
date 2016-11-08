//
//  speech.hpp
//  digital
//
//  Created by Bryce on 10/6/16.
//
//

#ifndef speech_hpp
#define speech_hpp

#include <stdio.h>
#include "speechbubble.hpp"

class SpeechManager {
protected:
    vector<unique_ptr<SpeechBubble>> speeches;
    vector<unique_ptr<SpeechBubble>> newSpeeches;
public:
    SpeechManager() {}
    
    void say(Entity *speaker, string msg);
    void say(unique_ptr<SpeechBubble> speech);
    void update();
    void draw();
    
    bool onMouseDown(MouseEvent event);
    bool onMouseUp(MouseEvent event);
    bool onMouseDrag(MouseEvent event);

};

#endif /* speech_hpp */

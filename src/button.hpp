//
//  button.hpp
//  digital
//
//  Created by Bryce on 11/2/16.
//
//

#ifndef button_hpp
#define button_hpp

#include <stdio.h>
#include "cinder/gl/gl.h"
#include "widget.hpp"

using namespace cinder;

class Button : public Widget {
protected:
    ColorA pressedColor;
    ColorA releasedColor;
    bool pressed;
public:
    Button();
    bool onMouseDown(MouseEvent event) override;
    bool onMouseUp(MouseEvent event) override;
    virtual void onClicK() {}
    void draw() override;
    
    bool isPressed() {return pressed;}
};

class TextButton : public Button {
    string text;
public:
    TextButton(string text);
    void setText(string text) {this->text = text;}
    void draw() override;
};

#endif /* button_hpp */

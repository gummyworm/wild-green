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
#include "cinder/gl/TextureFont.h"

using namespace cinder;

class Button : public Widget {
protected:
    struct ButtonFlags {
        bool pressed;
    }flags;

    ColorA pressedColor;
    ColorA releasedColor;
    function<void()> onClick;
public:
    Button(Widget *parent=nullptr, function<void()> onClick=nullptr);
    bool onMouseDown(MouseEvent event) override;
    bool onMouseUp(MouseEvent event) override;
    void draw() override;
    
    bool isPressed() {return flags.pressed;}
    void setOnClick(function<void()> onClick) {this->onClick = onClick;}
};

class ToggleButton : public Button {
public:
    ToggleButton(Widget *parent=nullptr, function<void()> onClick=nullptr);
    
    void draw() override;
    bool onMouseDown(MouseEvent event) override;
    bool onMouseUp(MouseEvent event) override;
};

class TextButton : public Button {
    string text;
public:
    TextButton(Widget *parent=nullptr, string text="", function<void()> onClick=nullptr);
    void setText(string text) {this->text = text;}
    void draw() override;
};

#endif /* button_hpp */

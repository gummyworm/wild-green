//
//  textedit.hpp
//  digital
//
//  Created by Bryce on 11/21/16.
//
//

#ifndef textedit_hpp
#define textedit_hpp

#include <stdio.h>
#include "widget.hpp"
#include "window.hpp"
#include "cinder/gl/TextureFont.h"

class TextWidget : public Widget {
protected:
    string text;
    gl::TextureFontRef font;
public:
    TextWidget(Widget *parent=nullptr);
    void append(string text);

    gl::TextureFontRef getFont() {return font;}
    void draw() override;
};

class TextEdit : public TextWidget {
protected:
public:
    TextEdit(Widget *parent=nullptr);
    
    bool onKeydown(KeyEvent event) override;
};

class TextWindow : public WidgetWindow {
protected:
    string text;
    gl::TextureFontRef font;
    float lineSize;
public:
    TextWindow(string name);
    void append(string text);
    
    gl::TextureFontRef getFont() {return font;}
    void draw() override;
};

class TextLog : public TextWindow {
protected:
public:
    TextLog(string name="");
};

#endif /* textedit_hpp */

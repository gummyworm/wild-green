//
//  textedit.cpp
//  digital
//
//  Created by Bryce on 11/21/16.
//
//

#include "textedit.hpp"
#include "properties.h"

TextWidget::TextWidget(Widget *parent)
:Widget(parent),
text()
{
    font = gl::TextureFont::create(properties::font);
}

void TextWidget::draw()
{
    Widget::draw();
    
    string line = "";
    ivec2 cursor = ivec2();
    
    gl::drawSolidRect(getRect());
    gl::color(1,1,1,1);
    for(char c : text) {
        if(c == '\n') {
            font->drawString(line, cursor);
            cursor.y += font->measureString(line).y;
            line.clear();
        } else {
            line += c;
        }
    }
    if(!line.empty()) {
        font->drawString(line, cursor);
    }
}

void TextWidget::append(string text)
{
    for(auto c : text) {
        string add(1, c);
        switch(c) {
            case '\b':
                this->text.pop_back();
                break;
            default:
                this->text.append(add);
                break;
        }

    }
}

TextEdit::TextEdit(Widget *parent)
:TextWidget(parent)
{
    
}

TextWindow::TextWindow(string name)
:WidgetWindow(name)
{
    
}

void TextWindow::draw()
{
    WidgetWindow::draw();
    
    string line = "";
    ivec2 cursor = ivec2();
    
    gl::drawSolidRect(getRect());
    gl::color(1,1,1,1);
    gl::translate(getInternalPos2D() - getPos2D());
    gl::translate(vec2(0,lineSize));
    for(char c : text) {
        if(c == '\n') {
            font->drawString(line, cursor);
            cursor.y += font->measureString(line).y;
            line.clear();
        } else {
            line += c;
        }
    }
    if(!line.empty()) {
        font->drawString(line, cursor);
    }
}

void TextWindow::append(string text)
{
    for(auto c : text) {
        string add(1, c);
        switch(c) {
            case '\b':
                this->text.pop_back();
                break;
            default:
                this->text.append(add);
                break;
        }
        
    }
}

bool TextEdit::onKeydown(KeyEvent event)
{
    if(event.getCode() == KeyEvent::KEY_RETURN) {
        this->text += '\n';
    } else {
        this->text += event.getChar();
    }
    return false;

}

TextLog::TextLog(string name)
:TextWindow(name)
{
    font = gl::TextureFont::create(properties::font);
    lineSize = getFont()->measureString("WWW").y;
}
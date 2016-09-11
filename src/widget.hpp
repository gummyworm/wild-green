//
//  widget.hpp
//  pool
//
//  Created by Bryce on 8/20/16.
//
//

#ifndef widget_hpp
#define widget_hpp

#include <stdio.h>
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Widget {
protected:
    ivec2 dim;
    ivec3 pos;
    
    bool show;
    
    int fontSize;
    ColorA bgColor;
    ColorA fontColor;
public:
    Widget();
    Widget(int width, int height);
    Widget(Rectf bounds);

    virtual void draw() {}
    void setPos(ivec2 pos);
    void setRect(Rectf rect);
    void resize(ivec2 pos);

    void setVisible(bool show) {this->show = show;}
    void setFontSize(int size) {fontSize = size;}
    
    Rectf getRect();
    bool isVisible() {return show;}
    
    virtual void onMouseDown(MouseEvent event) {}
    virtual void onMouseUp(MouseEvent event) {}
    virtual void onMouseMove(MouseEvent event) {}
    virtual void onMouseDrag(MouseEvent event) {}
    virtual void onKeydown(KeyEvent event) {}
    virtual void onAccept(MouseEvent event, shared_ptr<class Entity> e) {}
    
    ivec3 getPos() {return pos;}
    ivec2 getPos2D() {return ivec2(pos.x, pos.y);}
    ivec2 getDim() {return dim;}
};

#endif /* widget_hpp */

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
    const int numZLevels = 1024;
    ivec2 dim;
    ivec2 minDim, maxDim;
    ivec3 pos;

    bool grabbed;
    ivec2 grabbedPos;
    
    bool show;
    
    int fontSize;
    ColorA bgColor;
    ColorA fontColor;
    
    vector<Widget> children;

    void grab(ivec2 offset) {grabbed = true; grabbedPos = offset;}
    void ungrab() {grabbed = false;}
public:
    Widget(int width = 96, int height = 128);

    virtual void draw() {}
    virtual void resize(ivec2 size) {dim.x = size.x; dim.y = size.y;}
    void setPos(ivec2 pos);
    void setRect(Rectf rect);

    void setVisible(bool show) {this->show = show;}
    void setFontSize(int size) {fontSize = size;}
    
    void pullToFront();
    void pushDown();
    
    Rectf getRect();
    bool isVisible() {return show;}
    
    virtual void onMouseDown(MouseEvent event);
    virtual void onMouseUp(MouseEvent event) {}
    virtual void onMouseMove(MouseEvent event) {}
    virtual void onMouseDrag(MouseEvent event) {}
    virtual void onKeydown(KeyEvent event) {}
    virtual void onAccept(MouseEvent event, shared_ptr<class Entity> e) {}
    virtual void update() {}
    
    ivec3 getPos() {return pos;}
    ivec2 getPos2D() {return ivec2(pos.x, pos.y);}
    ivec2 getDim() {return dim;}
    float getAspectRatio() {return ((float)dim.x) / dim.y;}
    
    void setBgColor(ColorA color) {bgColor = color;}
    void setFontColor(ColorA color) {fontColor = color;}
    
    bool operator < (const Widget& w) const
    {
        return (pos.z < w.pos.z);
    }
};

#endif /* widget_hpp */

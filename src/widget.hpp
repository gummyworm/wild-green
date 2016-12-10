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
#include "cinder/gl/TextureFont.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Widget {
protected:
    struct Flags {
        bool drawBorder;
        bool drawBg;
        bool scissor;
    } flags;
    
    Widget *parent;
    
    const int numZLevels = 256;
    ivec2 dim;
    ivec2 minDim, maxDim;
    ivec3 pos;

    bool grabbed;
    ivec2 grabbedPos;
    
    bool show;
    
    int fontSize;
    gl::TextureFontRef font;
    ColorA bgColor;
    ColorA fontColor;
    
    vector<shared_ptr<Widget>> children;
    vector<Widget*> toRemove;
    vector<shared_ptr<Widget>> toAdd;
    
    // priority provides a hint to the ordering of widgets when rendering.
    int priority;
    
    Rectf border;
    void applyBorderOffset();
public:
    Widget(Widget *parent=nullptr, int width = 96, int height = 128, bool borderless = false);
    
    virtual void addChild(shared_ptr<Widget> child);
    void removeChild(Widget *child);
    shared_ptr<Widget> getChild(Widget *w);

    // draw renders this widget.
    virtual void draw();
    
    // apply performs cleanup and finishes a widget's draw.
    virtual void apply();
    
    virtual void resize(ivec2 size) {dim.x = size.x; dim.y = size.y;}
    void setPos(ivec2 pos);
    void setPos(ivec3 pos);
    void setRect(Rectf rect);

    void setVisible(bool show) {this->show = show;}
    void setFontSize(int size) {fontSize = size;}
    
    void pullToFront();
    void pushDown();
    void reparent(Widget *w);
    
    Rectf getRect();
    bool isVisible() {return show;}
    
    virtual bool onMouseDown(MouseEvent event);
    virtual bool onMouseUp(MouseEvent event);
    virtual bool onMouseMove(MouseEvent event) {return false;}
    virtual bool onMouseDrag(MouseEvent event);
    virtual bool onKeydown(KeyEvent event) {return false;}
    virtual bool onAccept(MouseEvent event, shared_ptr<class Entity> e) {return false;}
    virtual bool onAccept(MouseEvent, shared_ptr<Widget> w) {return false;}
    virtual void update();
    
    ivec3 getPos() {return pos;}
    ivec2 getPos2D() {return ivec2(pos.x, pos.y);}
    ivec3 getAbsPos() {
        ivec3 pos = getPos();
        for(auto p = parent; p != nullptr; p = p->parent)
            pos += p->getPos();
        return pos;
    }
    
    ivec2 getAbsPos2D() {
        ivec2 pos = getPos2D();
        for(auto p = parent; p != nullptr; p = p->parent)
            pos += p->getPos2D();
        return pos;
    }
    
    void setAbsPos(ivec3 pos) {
        ivec3 p = ivec3();
        if(parent != nullptr)
            p = parent->getAbsPos();
        setPos(pos - p);
    }
    
    void grab(ivec2 offset) {
        grabbed = true;
        grabbedPos = offset - getAbsPos2D();
        pullToFront();
    }
    
    void ungrab() {
        grabbed = false;
    }

    void setAbsPos2D(ivec2 pos) {
        setAbsPos(ivec3(pos.x, pos.y, this->pos.z));
    }
    
    ivec2 getDim() {return dim;}
    
    float getAspectRatio() {return ((float)dim.x) / dim.y;}
    
    bool contains(vec2 pos);
    
    void setBgColor(ColorA color) {bgColor = color;}
    void setFontColor(ColorA color) {fontColor = color;}
    
    bool operator < (const Widget& w) const
    {
        return (pos.z < w.pos.z);
    }
};

#endif /* widget_hpp */

//
//  window.hpp
//  digital
//
//  Created by Bryce on 8/25/16.
//
//

#ifndef window_hpp
#define window_hpp

#include <stdio.h>
#include <string.h>
#include "cinder/gl/gl.h"
#include "widget.hpp"
#include "properties.h"

using namespace cinder;

class WidgetWindow : public Widget {
protected:
    class TitleBar {
    public:
        gl::TextureFontRef font;

        TitleBar(string name):text(name),height(properties::windowTitleHeight) {
            font = gl::TextureFont::create(properties::windowTitleFont);
        }
        string text;
        int height;
    };
    
    class Scrollbar {
        bool enabled;
        bool horiz;
        int size;
        int minSize;
        int scroll;
        bool grabbed;
        
        WidgetWindow *parent;
        
        gl::TextureRef upArrow, downArrow, leftArrow, rightArrow;
        
        Rectf getLeftArrow();
        Rectf getRightArrow();
        Rectf getDownArrow();
        Rectf getUpArrow();
        Rectf getBarRect();
        
        
    public:
        Scrollbar(WidgetWindow* parent, bool horiz=false, int size=10):parent(parent), horiz(horiz), minSize(size), scroll(0) {
            upArrow = gl::Texture::create(loadImage(loadAsset("uparrow.png")));
            downArrow = gl::Texture::create(loadImage(loadAsset("downarrow.png")));
            leftArrow = gl::Texture::create(loadImage(loadAsset("leftarrow.png")));
            rightArrow = gl::Texture::create(loadImage(loadAsset("rightarrow.png")));
        }
        
        int getScroll() {return scroll;}
        bool getVisible() {return true;} //scroll > 0;}
        
        void draw(Rectf parentRect);
        void onMouseDown(MouseEvent event, Rectf winRect);
        void onMouseUp(MouseEvent event);
        void onMouseDrag(MouseEvent event);
    };
    
    TitleBar title;
    bool grabbed;
    bool resizing;
    ivec2 prevMouse;
    vec2 closeButtonOffset;
    Rectf closeButton;
    
    bool resizable;
    
    ivec2 virtualSize;
    Scrollbar hScroll, vScroll;

    Rectf getResizeRect();
    Rectf getBringFrontRect();
    gl::TextureRef resizeIcon;
    
    Rectf getBringFrontIcon();
    gl::TextureRef bringFrontIcon;
public:
    WidgetWindow(string name, int width = 320, int height = 200);
    
    void draw() override;
    bool onMouseDown(MouseEvent event) override;
    bool onMouseUp(MouseEvent event) override;
    bool onMouseMove(MouseEvent event) override;
    bool onMouseDrag(MouseEvent event) override;
    bool onAccept(MouseEvent event, shared_ptr<class Entity> e) override;
    void setTitle(string title);
    
    Rectf getInternalRect();
    ivec2 getInternalPos2D() {return ivec2(pos.x, pos.y+title.height);}
    void setVirtualSize(ivec2 size) {virtualSize = size;}
    ivec2 getVirtualSize() {return virtualSize;}
    ivec2 getScroll() {return ivec2(hScroll.getScroll(), vScroll.getScroll());}
};

#endif /* window_hpp */

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

using namespace cinder;

class WidgetWindow : public Widget {
protected:
    class TitleBar {
    public:
        TitleBar(string name):text(name),height(20) {}
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
        
        gl::TextureRef upArrow, downArrow, leftArrow, rightArrow;
        
        Rectf getLeftArrow(Rectf winRect);
        Rectf getRightArrow(Rectf winRect);
        Rectf getDownArrow(Rectf winRect);
        Rectf getUpArrow(Rectf winRect);
        Rectf getBarRect(Rectf winRect);
    public:
        Scrollbar(bool horiz=false, int size=10):horiz(horiz), minSize(size) {
            upArrow = gl::Texture::create(loadImage(loadAsset("uparrow.png")));
            downArrow = gl::Texture::create(loadImage(loadAsset("downarrow.png")));
            leftArrow = gl::Texture::create(loadImage(loadAsset("leftarrow.png")));
            rightArrow = gl::Texture::create(loadImage(loadAsset("rightarrow.png")));
        }
        
        int getScroll() {return scroll;}
        
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

    Rectf getResizeRect(Rectf winRect);
    Rectf getBringFrontRect(Rectf winRect);
    gl::TextureRef resizeIcon;
    
    Rectf getBringFrontIcon(Rectf winRect);
    gl::TextureRef bringFrontIcon;

public:
    WidgetWindow(string name, int width = 320, int height = 200);
    
    void draw() override;
    void onMouseDown(MouseEvent event) override;
    void onMouseUp(MouseEvent event) override;
    void onMouseMove(MouseEvent event) override;
    void onMouseDrag(MouseEvent event) override;
    void setTitle(string title);
    
    Rectf getInternalRect();
    ivec2 getInternalPos2D() {return ivec2(pos.x, pos.y+title.height);}
};

#endif /* window_hpp */

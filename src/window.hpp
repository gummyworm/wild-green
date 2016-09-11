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
#include "widget.hpp"

class WidgetWindow : public Widget {
protected:
    class TitleBar {
    public:
        TitleBar(string name):text(name),height(20) {}
        string text;
        int height;
    };
    
    TitleBar title;
    bool grabbed;
    ivec2 prevMouse;
    vec2 closeButtonOffset;
    Rectf closeButton;
    
    Rectf getInternalRect();
    ivec2 getInternalPos2D() {return ivec2(pos.x, pos.y+title.height);}
public:
    WidgetWindow(string name, int width = 320, int height = 200);
    
    void draw() override;
    void onMouseDown(MouseEvent event) override;
    void onMouseUp(MouseEvent event) override;
    void onMouseMove(MouseEvent event) override;
    void onMouseDrag(MouseEvent event) override;
    void setTitle(string title);
};

#endif /* window_hpp */

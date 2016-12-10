//
//  ButtonGrid.cpp
//  digital
//
//  Created by Bryce on 11/10/16.
//
//

#include "ButtonGrid.hpp"

const int sx = 32;
const int sy = 32;

ButtonGrid::ButtonGrid(Widget *parent, int w, int h, vector<shared_ptr<Button>> buttons)
:Widget(parent),
selected(nullptr),
selectedIndex(0),
rows(h),
cols(w)
{
    layout();
    resize(ivec2(sx*w, sy*h));
}

void ButtonGrid::layout()
{
    int i = 0;
    for(auto w : children) {
        int row = i / rows;
        int col = i % rows;
        w->setPos(ivec2(sx*row, sy*col));
        w->resize(ivec2(sx, sy));
        ++i;
    }
}

bool ButtonGrid::onMouseDown(MouseEvent event)
{
    int i = 0;
    
    for(auto b : children) {
        if(b->onMouseDown(event)) {
            selected = b;
            selectedIndex = i;
            return true;
        }
        i++;
    }
    return false;
}

bool ButtonGrid::onMouseUp(MouseEvent event)
{
    for(auto b : children) {
        if(b->onMouseUp(event)) {
            return true;
        }
    }
    return false;
}

void ButtonGrid::draw()
{
    Widget::draw();
    layout();
    for(auto w : children) {
        w->draw();
        w->apply();
    }
}

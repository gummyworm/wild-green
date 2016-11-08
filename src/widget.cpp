//
//  widget.cpp
//  pool
//
//  Created by Bryce on 8/20/16.
//
//

#include "widget.hpp"
#include "properties.h"
#include "gui.h"

Widget::Widget(Widget *parent, int width, int height, bool borderless)
:parent(parent),
dim(width, height),
pos(20,20,0),
fontSize(32),
fontColor(1,0,1,1),
bgColor(0.5f,0.5f,0.5f,1.0f),
minDim(32,32),
maxDim(640,480),
show(true)
{
    flags.drawBorder = !borderless;
    setPos(pos);
}

void Widget::setPos(ivec2 pos)
{
    setPos(ivec3(pos.x, pos.y, this->pos.z));
}

void Widget::setPos(ivec3 pos)
{
    this->pos = pos;
    for(auto c : children) {
        c->setPos(c->getPos());
    }
}

Rectf Widget::getRect()
{
    return Rectf(0, 0, dim.x, dim.y);
}

bool Widget::contains(vec2 pt)
{
    ivec3 absPos = getAbsPos();
    return Rectf(absPos.x, absPos.y, absPos.x+dim.x, absPos.y+dim.y).contains(pt);
}

void Widget::pullToFront()
{
    pos.z = 0;
}

void Widget::pushDown()
{
    pos.z--;
}

bool Widget::onMouseDown(MouseEvent event)
{
    if(getRect().contains(event.getPos())) {
        pullToFront();
        return true;
    }
    for(auto c : children) {
        if(c->onMouseDown(event))
            return true;
    }
    return false;
}

void Widget::addChild(shared_ptr<Widget> child)
{
    children.push_back(child);
}

void Widget::draw()
{
    gl::pushMatrices();
    gl::translate(getPos2D());

    if(!isVisible())
        return;
    
    if(flags.drawBorder) {
        gl::color(gui::borderColor);
        gl::drawStrokedRect(getRect(), gui::borderWidth);
    }
    
    gl::color(1,1,1,1);
}

void Widget::apply()
{
    gl::popMatrices();
}
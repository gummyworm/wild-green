//
//  widget.cpp
//  pool
//
//  Created by Bryce on 8/20/16.
//
//

#include "widget.hpp"
#include "properties.h"

Widget::Widget()
:dim(100,100),
pos(0,0,0),
fontSize(32),
fontColor(1,0,1,1),
bgColor(0.5f,0.5f,0.5),
show(true)
{
}

Widget::Widget(int width, int height)
:dim(width, height)
{
}

Widget::Widget(Rectf bounds)
:pos(bounds.x1, bounds.y1, 0),
dim(bounds.getWidth(), bounds.getHeight())
{
}

void Widget::setPos(ivec2 pos)
{
    this->pos.x = pos.x;
    this->pos.y = pos.y;
}

void Widget::resize(ivec2 size)
{
    dim.x = size.x;
    dim.y = size.y;
}

Rectf Widget::getRect()
{
    return Rectf(pos.x, pos.y, pos.x+dim.x, pos.y+dim.y);
}
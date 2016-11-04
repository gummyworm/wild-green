//
//  widget.cpp
//  pool
//
//  Created by Bryce on 8/20/16.
//
//

#include "widget.hpp"
#include "properties.h"

Widget::Widget(int width, int height)
:dim(100,100),
pos(20,20,0),
fontSize(32),
fontColor(1,0,1,1),
bgColor(0.5f,0.5f,0.5f,1.0f),
minDim(32,32),
maxDim(640,480),
show(true)
{
}

void Widget::setPos(ivec2 pos)
{
    this->pos.x = pos.x;
    this->pos.y = pos.y;
}

Rectf Widget::getRect()
{
    return Rectf(pos.x, pos.y, pos.x+dim.x, pos.y+dim.y);
}

void Widget::pullToFront()
{
    pos.z = numZLevels;
}

void Widget::pushDown()
{
    pos.z++;
}

void Widget::onMouseDown(MouseEvent event)
{
    if(getRect().contains(event.getPos())) {
        pullToFront();
    }
}
//
//  verticallayout.cpp
//  digital
//
//  Created by Bryce on 11/19/16.
//
//

#include "verticallayout.hpp"

VerticalLayout::VerticalLayout()
:Widget(),
cursor()
{

}

void VerticalLayout::addChild(shared_ptr<Widget> child)
{
    Widget::addChild(child);
    child->setPos(cursor);
    cursor.y += child->getDim().y;
}
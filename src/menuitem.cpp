//
//  menuitem.cpp
//  digital
//
//  Created by Bryce on 9/20/16.
//
//

#include "menu.hpp"

void Menu::draw()
{
    Widget::draw();
    ivec2 cur = getPos2D();
    
    for(auto i : items) {
        float yAdvance = font->measureString("XXX").y;
        gl::drawString(i.getName(), cur);
        cur.y += yAdvance;
    }
}
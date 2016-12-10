//
//  widget.cpp
//  pool
//
//  Created by Bryce on 8/20/16.
//
//

#include "widget.hpp"
#include "properties.h"
#include "game.h"
#include "gui.h"

Widget::Widget(Widget *p, int width, int height, bool borderless)
:parent(p),
dim(width, height),
pos(0,0,0),
fontSize(32),
fontColor(1,0,1,1),
bgColor(0.5f,0.5f,0.5f,1.0f),
minDim(32,32),
maxDim(640,480),
show(true),
grabbed(false),
priority(-1)
{
    flags.drawBorder = !borderless;
    flags.scissor = false;
    flags.drawBg = true;
    setPos(pos);
    
    if(parent != nullptr)
        parent->children.push_back(shared_ptr<Widget>(this));
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
    pos.z++;
}

void Widget::pushDown()
{
    pos.z--;
}

bool Widget::onMouseDown(MouseEvent event)
{
    if(getRect().contains(event.getPos())) {
        pullToFront();
    }
    for(auto c : children) {
        if(c->onMouseDown(event))
            return true;
    }
    return false;
}

bool Widget::onMouseUp(MouseEvent event)
{
    grabbed = false;
    return false;
}

bool Widget::onMouseDrag(MouseEvent event)
{
    if(grabbed) {
        setAbsPos2D(event.getPos() - grabbedPos);
        return true;
    }
    return false;
}

void Widget::addChild(shared_ptr<Widget> child)
{
    if(child == nullptr)
        return;
    child->parent = this;
    toAdd.push_back(child);
}

void Widget::removeChild(Widget *child)
{
    toRemove.push_back(child);
    child->parent = nullptr;
}

shared_ptr<Widget> Widget::getChild(Widget *w)
{
    for(auto c : children) {
        if(c.get() == w)
            return c;
    }
    return nullptr;
}

void Widget::reparent(Widget *w)
{
    if(parent == w)
        return;
    
    ivec3 p = getAbsPos();
    shared_ptr<Widget> c = nullptr;
    
    if(parent != nullptr) {
        c = parent->getChild(this);
        parent->removeChild(this);
    } else if(game::guiMgr.getWidget(this) != nullptr) {
        parent = nullptr;
        c = game::guiMgr.getWidget(this);
        game::guiMgr.remove(this);
    }
    
    if(w != nullptr) {
        w->addChild(c);
    } else {
        game::guiMgr.addWidget(c);
    }
    
    setAbsPos(p);
}

static bool sortZ(shared_ptr<Widget> w1, shared_ptr<Widget> w2)
{
    return w1->getPos().z > w2->getPos().z;
}

void Widget::update()
{
    vector<shared_ptr<Widget>> updated;
    for(auto w : children) {
        if(find(toRemove.begin(), toRemove.end(), w.get()) == toRemove.end()) {
            w->update();
            updated.push_back(w);
        }
    }
    toRemove.clear();
    children = updated;
    
    for(auto nw : toAdd)
        children.push_back(nw);
    toAdd.clear();
    
    sort(children.begin(), children.end(), sortZ);
    int i = 0;
    for(auto w : children) {
        ivec3 pos = w->getPos();
        pos.z = i;
        w->setPos(pos);
    }
}

void Widget::draw()
{
    gl::pushMatrices();

    if(!isVisible())
        return;
    
    gl::translate(getPos2D());
    if(flags.drawBorder) {
        gl::color(gui::borderColor);
        gl::drawStrokedRect(getRect() + Rectf(-gui::borderWidth/2, -gui::borderWidth/2, gui::borderWidth/2, gui::borderWidth/2), gui::borderWidth);
    }
    
    if(flags.scissor) {
        gl::enable(GL_SCISSOR_TEST);
        vec2 scale = vec2((float)app::getWindowWidth() / properties::screenWidth, (float)app::getWindowHeight() / properties::screenHeight);
        int x = getAbsPos2D().x;
        int y= properties::screenHeight - (getAbsPos2D().y + dim.y);
        gl::scissor(scale.x * x, scale.y * y, scale.x * dim.x, scale.y * dim.y);
    }
    
    if(flags.drawBg) {
        gl::color(gui::windowBgColor);
        gl::drawSolidRect(getRect());
    }
    
    gl::color(1,1,1,1);
}

void Widget::apply()
{
    gl::popMatrices();
}
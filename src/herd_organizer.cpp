//
//  herd_organizer.cpp
//  digital
//
//  Created by Bryce on 12/3/16.
//
//

#include "herd_organizer.hpp"
#include "game.h"

HerdOrganizer::HerdOrganizer(shared_ptr<Entity> leader, int w, int h)
:PRGLauncher("SHEPARD"),
gridDim(ivec2(w, h)),
leader(leader) {

}

void HerdOrganizer::launch()
{
    game::guiMgr.addWidget(shared_ptr<HerdOrganizerWin>(new HerdOrganizerWin(leader, gridDim.x, gridDim.y)));
}

HerdOrganizerWin::HerdOrganizerWin(shared_ptr<Entity> leader, int w, int h)
:PRG("SHEPARD.PRG"),
leader(leader)
{
    for(int i = 0; i < h; ++i) {
        grid.push_back(vector<shared_ptr<Tile>>(w));
        for(int j = 0; j < w; ++j) {
            vec3 offset = vec3(-(j-w/2), 0, -(i-h/2));
            grid[i][j] = shared_ptr<Tile>(new Tile(leader, offset));
        }
    }
}

void HerdOrganizerWin::Tile::draw(vec2 dim)
{
    if(entity == nullptr)
        return;
    
    // draw the entity thumbnail
    auto thumb = entity->getThumbnail();
    if(thumb != nullptr) {
        gl::draw(thumb, Area(vec2(), thumb->getSize()), Area(vec2(), dim));
    } else {
        gl::drawString(entity->getName(), vec2());
    }
    
    // draw the AI thumbnail
    auto aiThumb = aiPattern->getIcon();
    if(aiThumb != nullptr) {
        gl::draw(aiThumb, Area(vec2(), aiThumb->getSize()), Area(vec2(), dim));
    }
}

void HerdOrganizerWin::Tile::update()
{
    if(entity == nullptr)
        return;
    aiPattern->update();
}

shared_ptr<HerdOrganizerWin::Tile> HerdOrganizerWin::getTile(ivec2 pos)
{
    float xStart = getInternalRect().x1 + 4;
    float yStart = getInternalRect().y1 + 4;
    float xStop = getInternalRect().getWidth()-4;
    float yStop = getInternalRect().getHeight()-4;

    float xStep = ((float)(xStop - xStart)) / (float)grid.size();
    float yStep = ((float)(yStop - yStart)) / (float)grid[0].size();

    int x = (pos.x - xStart)/xStep;
    int y = (pos.y - yStart)/yStep;
    
    if(x >= grid[0].size() || y >= grid.size())
        return nullptr;
    
    return grid[y][x];
}

shared_ptr<HerdOrganizerWin::Tile> HerdOrganizerWin::getTile(shared_ptr<Entity> e)
{
    for(auto row : grid) {
        for(auto t : row) {
            if(t->entity == e)
                return t;
        }
    }
    return nullptr;
}

void HerdOrganizerWin::update()
{
    for(auto row : grid) {
        for(auto t : row) {
            t->update();
        }
    }
}

void HerdOrganizerWin::draw()
{
    PRG::draw();
    
    float xStart = getInternalRect().x1 + 4;
    float yStart = getInternalRect().y1 + 4;
    float xStop = getInternalRect().getWidth()-4;
    float yStop = getInternalRect().getHeight()-4;
    
    float xStep = (xStop - xStart) / grid.size();
    float yStep = (yStop - yStart) / grid[0].size();
    
    gl::lineWidth(4.0f);
    gl::color(0,0,0,1);
    
    // draw vertical lines
    for(int i = 0; i <= grid.size(); ++i) {
        Rectf r(xStart + xStep*i, yStart, xStart+4 + xStep*i, yStop+4);
        gl::drawSolidRect(r);
    }
    
    // draw horizontal lines
    for(int i = 0; i <= grid[0].size(); ++i) {
        Rectf r(xStart, yStart + yStep*i, xStop+4, yStart+4 + yStep*i);
        gl::drawSolidRect(r);
    }
    
    // draw player location indicator
    gl::color(1,0,1,1);
    float a = 3.1415926536 - mainCam.getRot().y;
    vec2 p0 = vec2(xStart+2+(xStop-xStart)/2.0f - sin(a)*8,
                   yStart+(yStop-yStart)/2.0f + cos(a)*8);
    vec2 p1 = vec2(xStart+2+(xStop-xStart)/2.0f + cos(a)*-4 - sin(a)*-8,
                   yStart+(yStop-yStart)/2.0f + sin(a)*-4 + cos(a)*-8);
    vec2 p2 = vec2(xStart+2+(xStop-xStart)/2.0f + cos(a)*4 - sin(a)*-8,
                   yStart+(yStop-yStart)/2.0f + sin(a)*4 + cos(a)*-8);
    gl::drawSolidTriangle(p0, p1, p2);
    
    // draw tiles
    gl::pushMatrices();
    gl::translate(xStart, yStart, 0.0f);
    for(int i = 0; i < grid.size(); ++i) {
        gl::pushMatrices();
        for(int j = 0; j < grid[0].size(); ++j) {
            if(grid[i][j] != grabbedTile)
                grid[i][j]->draw(vec2(xStep, yStep));
            gl::translate(xStep, 0.0f, 0.0f);
        }
        gl::popMatrices();
        gl::translate(0.0f, yStep, 0.0f);
    }
    gl::popMatrices();
    
    if(grabbedTile != nullptr) {
        gl::pushMatrices();
        gl::translate(grabbedPos);
        grabbedTile->draw(vec2(xStep+15, yStep+15));
        gl::popMatrices();
    }
}

bool HerdOrganizerWin::onMouseDown(MouseEvent event)
{
    if(WidgetWindow::onMouseDown(event))
        return true;
    
    auto t = getTile(event.getPos() - getPos2D());
    if(t == nullptr)
        return false;
    
    if(t->entity != nullptr) {
        if(event.isLeft()) {
            grabbedTile = t;
            grabbedPos = event.getPos() - getPos2D();
        } else if(event.isRight()) {
            auto m = t->entity->getActions();
            m->onMouseDown(event);
            game::guiMgr.addWidget(m);
            
        }
    } else {
        grabbedTile = nullptr;
    }
    return false;
}

bool HerdOrganizerWin::onMouseUp(MouseEvent event)
{
    if(WidgetWindow::onMouseUp(event))
        return true;
    
    auto t = getTile(event.getPos() - getPos2D());
    if(t != nullptr && grabbedTile != nullptr) {
        auto old = getTile(grabbedTile->entity);
        if(t->entity == nullptr) {
            if(event.isLeft()) {
                t->setEntity(grabbedTile->entity);
                old->entity = nullptr;
                old->aiPattern->possess(nullptr);
            }
        }
    }
    
    grabbedTile = nullptr;
    
    return false;
}

bool HerdOrganizerWin::onMouseDrag(MouseEvent event)
{
    if(WidgetWindow::onMouseDrag(event))
        return true;
    grabbedPos = event.getPos() - getPos2D();
    return false;
}

bool HerdOrganizerWin::onAccept(MouseEvent event, shared_ptr<class Entity> e)
{
    if(e == nullptr)
        return false;
    
    auto t = getTile(event.getPos() - getPos2D());
    if(t == nullptr)
        return false;
    
    auto old = getTile(e);
    if(old != nullptr)
        old->entity = nullptr;
    
    if(t != nullptr && e != nullptr) {
        t->setEntity(e);
    }
    return false;
}

bool HerdOrganizerWin::onAccept(MouseEvent, shared_ptr<Widget> w)
{
    return false;
}
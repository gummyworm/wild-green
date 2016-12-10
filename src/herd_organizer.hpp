//
//  herd_organizer.hpp
//  digital
//
//  Created by Bryce on 12/3/16.
//
//

#ifndef herd_organizer_hpp
#define herd_organizer_hpp

#include <stdio.h>
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Batch.h"
#include "prg.hpp"
#include "entity.hpp"
#include "ai_follow.hpp"
#include "contextmenu.hpp"

using namespace ci;
using namespace cinder;
using namespace ci::app;
using namespace std;


class HerdOrganizerWin : public PRG {
protected:
    class Tile {
    public:
        gl::TextureRef icon;
        shared_ptr<Entity> entity;
        shared_ptr<Entity> leader;

        shared_ptr<AIPattern> aiPattern;
        
        Tile(shared_ptr<Entity> leader, vec3 offset, string iconImage="")
        :leader(leader),
        icon(nullptr) {
            auto follow = shared_ptr<AIFollow>(new AIFollow(leader.get(), 0.5f, offset));
            follow->setSpeed(leader->getSpeed());
            aiPattern = follow;
            if(!iconImage.empty())
                icon = gl::Texture::create(loadImage(loadAsset(iconImage)));
        }
        void draw(vec2 dim);
        void update();
        void setEntity(shared_ptr<Entity> e) {
            entity = e;
            if(e != nullptr) {
                aiPattern->possess(entity.get());
            } else {
                aiPattern->possess(nullptr);
            }
        }
    };
    
    vector<vector<shared_ptr<Tile>>> grid;
    shared_ptr<Entity> leader;
    shared_ptr<Tile> getTile(ivec2 pos);
    shared_ptr<Tile> getTile(shared_ptr<Entity> e);
    shared_ptr<Tile> grabbedTile;
    vec2 grabbedPos;
public:
    HerdOrganizerWin(shared_ptr<Entity> leader, int w, int h);
    
    void draw() override;
    //void resize(ivec2 dim) override;
    //bool onKeydown(KeyEvent event) override;
    //bool onMouseDown(MouseEvent event) override;
    
    bool onMouseDown(MouseEvent event) override;
    bool onMouseUp(MouseEvent event) override;
    bool onMouseDrag(MouseEvent event) override;
    bool onAccept(MouseEvent event, shared_ptr<class Entity> e) override;
    bool onAccept(MouseEvent, shared_ptr<Widget> w) override;
    void update() override;
};

class HerdOrganizer : public PRGLauncher {
protected:
    ivec2 gridDim;
    shared_ptr<Entity> leader;
public:
    HerdOrganizer(shared_ptr<Entity> leader, int w=8, int h=8);
    void launch() override;
};


#endif /* herd_organizer_hpp */

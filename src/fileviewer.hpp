//
//  fileviewer.hpp
//  digital
//
//  Created by Bryce on 8/29/16.
//
//

#ifndef fileviewer_hpp
#define fileviewer_hpp

#include <stdio.h>
#include "prg.hpp"
#include "cinder/gl/gl.h"
#include "cinder/gl/texture.h"
#include "zap_spell.hpp"
#include "game.h"

using namespace ci::app;
using namespace std;
using namespace ci;

class File : public Widget {
protected:
    string name;
    string contents;
    struct {
        gl::TextureRef tex;
        Rectf texcos;
    } thumbnail;
    
    shared_ptr<PRG> program;
    shared_ptr<Entity> entity;
    
    Widget *detached;
    ivec2 detachedPos;
public:
    File(Widget *parent=nullptr, string name="", string thumbnailFilename="", shared_ptr<Entity> e=nullptr)
    :Widget(parent, 32, 32, true), name(name), entity(e), detached(parent) {
        if(thumbnailFilename.empty()) {
            thumbnailFilename = "geos-kernal.png";
        }
        thumbnail.tex = gl::Texture::create(loadImage(loadAsset(thumbnailFilename)));
        thumbnail.texcos = Rectf(0, 1, 1, 0);
        flags.drawBorder = false;
        setPos(ivec3(0, 0, 0));
    }
    
    void setThumbnail(gl::TextureRef tex, Rectf texcos=Rectf(0,1,1,0)) {thumbnail = {tex, texcos};}
    string getName() {return name;}
    
    bool onMouseDrag(MouseEvent event);
    bool onMouseDown(MouseEvent event);
    bool onMouseUp(MouseEvent event);
    
    void draw();
};


class SpellFile : public File {
protected:
    shared_ptr<Spell> spell;
public:
    SpellFile(Widget *parent=nullptr, string name="", string thumbnailFilename="", shared_ptr<Spell> spell=nullptr, shared_ptr<Entity> e=nullptr)
    :File(parent, name, thumbnailFilename, e), spell(spell) {}
    
    virtual bool activate(shared_ptr<Entity> target) {
        game::guiMgr.remove(this);
        if(target != nullptr && spell->onCast(mainCam.getPos(), target))
                return true;
        return spell->onCast(mainCam.getPos());
    }
    bool onMouseUp(MouseEvent event);
};

class ZapFile : public SpellFile {
protected:
    Zap spell;
public:
    ZapFile(Widget *parent = nullptr)
    :SpellFile(parent, "ZAP") {
    }
    bool activate(shared_ptr<Entity> target) override {
        return spell.onCast(mainCam.getPos());
    }
};

class FileViewerWin : public PRG {
protected:
    const ivec2 thumbnailSize = ivec2(48, 42);
    
    void arrange();
public:
    FileViewerWin(string name="");
    
    void addFile(shared_ptr<File> f);
    void draw() override;
    bool onMouseDown(MouseEvent event) override;
    bool onAccept(MouseEvent event, shared_ptr<class Entity> e) override;
    bool onMouseDrag(MouseEvent event) override;
};

class FileViewer : public PRGLauncher {
protected:
public:
    FileViewer(string name="File Viewer");
    void launch() override;
    bool onMouseDown(MouseEvent event) override;
};

#endif /* fileviewer_hpp */

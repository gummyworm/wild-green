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

using namespace ci::app;
using namespace std;
using namespace ci;

class File : public Widget {
    string name;
    string contents;
    ivec2 pos;
    struct {
        gl::TextureRef tex;
        Rectf texcos;
    } thumbnail;
    
    shared_ptr<PRG> program;
    Entity *entity;
public:
    File(Widget *parent=nullptr, string name="", string thumbnailFilename="", Entity *e=nullptr)
    :Widget(parent, 32, 32, true), name(name), entity(e) {
        thumbnail.tex = gl::Texture::create(loadImage(loadAsset(thumbnailFilename)));
        thumbnail.texcos = Rectf(0, 1, 1, 0);
        flags.drawBorder = false;
    }
    
    void setThumbnail(gl::TextureRef tex, Rectf texcos=Rectf(0,1,1,0)) {thumbnail = {tex, texcos};}
    ivec2 getPos() {return pos;}
    string getName() {return name;}
    
    bool onMouseDrag(MouseEvent event);
    bool onMouseDown(MouseEvent event);
    bool onMouseUp(MouseEvent event);
    
    void draw();
};

class FileViewerWin : public PRG {
protected:
    const ivec2 thumbnailSize = ivec2(48, 42);
    
    vector<shared_ptr<File>> files;
    shared_ptr<File> selected;
    
    void arrange();
public:
    FileViewerWin(string name="");
    
    void addFile(shared_ptr<File> f);
    void draw() override;
    bool onMouseDown(MouseEvent event) override;
    bool onMouseUp(MouseEvent event) override;
    bool onAccept(MouseEvent event, shared_ptr<class Entity> e) override;
    bool onMouseDrag(MouseEvent event) override;
};

class FileViewer : public PRGLauncher {
protected:
public:
    FileViewer();
    void launch() override;
    bool onMouseDown(MouseEvent event) override;
};

#endif /* fileviewer_hpp */

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

class File {
    string name;
    string contents;
    ivec2 pos;
    gl::TextureRef thumbnail;
    shared_ptr<PRG> program;
public:
    File(string name, string thumbnailFilename)
    :name(name) {
        thumbnail = gl::Texture::create(loadImage(loadAsset(thumbnailFilename)));
    }
    File(string name, string thumbnail, shared_ptr<PRG> exe)
    :File(name, thumbnail) {
        program = exe;
    }
    File(string name, string thumbnail, shared_ptr<PRG> exe, ivec2 pos)
    :File(name, thumbnail, exe) {
        this->pos = pos;
    }
    
    gl::TextureRef getThumbnail() {return thumbnail;}
    ivec2 getPos() {return pos;}
    string getName() {return name;}
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
    void onMouseDown(MouseEvent event) override;
    void onAccept(MouseEvent event, shared_ptr<class Entity> e) override;
};

class FileViewer : public PRGLauncher {
protected:
public:
    FileViewer();
    void launch() override;
    void onMouseDown(MouseEvent event) override;
};

#endif /* fileviewer_hpp */

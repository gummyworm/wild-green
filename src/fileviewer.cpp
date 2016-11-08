//
//  fileviewer.cpp
//  digital
//
//  Created by Bryce on 8/29/16.
//
//

#include "fileviewer.hpp"
#include "entity.hpp"
#include "game.h"
#include "properties.h"

bool File::onMouseDown(MouseEvent event)
{
    if(contains(event.getPos()) && parent->contains(parent->getPos2D())) {
        grab(event.getPos());
        return true;
    }
    if(contains(event.getPos()) && entity != nullptr) {
        grab(event.getPos());
        entity->generateShadow();
        return true;
    }
    return false;
}

bool File::onMouseUp(MouseEvent event)
{
    if(grabbed && entity != nullptr && !parent->contains(event.getPos())) {
        entity->setEnabled(true);
        entity->setPos(game::screenToWorld(event.getPos(), 5.0f));
        show = false;
    }
    ungrab();
    return false;
}

bool File::onMouseDrag(MouseEvent event)
{
    if(!grabbed)
        return false;
    
    if(parent != nullptr && parent->contains(event.getPos())) {
        if(entity != nullptr && !entity->isGrabbed()) {
            entity->setEnabled(false);
        }
    } else {
        show = true;
    }
    setAbsPos2D(event.getPos()-grabbedPos);

    return true;
}

void File::draw()
{
    Widget::draw();
    if(!show)
        return;
    
    //gl::draw(getThumbnail(), getRect());
    gl::enable(GL_TEXTURE_2D);
    
    gl::ScopedGlslProg scpGlslProg(gl::getStockShader(gl::ShaderDef().texture()));
    gl::ScopedTextureBind scpTextureBind(thumbnail.tex, 0);
    gl::drawSolidRect(getRect(),
                      vec2(thumbnail.texcos.x1, thumbnail.texcos.y1),
                      vec2(thumbnail.texcos.x2, thumbnail.texcos.y2));
}

FileViewerWin::FileViewerWin(string name)
:PRG(name),
files()
{
    shared_ptr<File> f(new File(this, "test", "geos-kernal.png"));
    addFile(f);
}

void FileViewerWin::addFile(shared_ptr<File> f)
{
    files.push_back(f);
    addChild(f);
}

void FileViewerWin::draw()
{
    WidgetWindow::draw();
    if(!show)
        return;
    
    for(auto f : files) {
        f->draw();
        f->apply();
    }
}

bool FileViewerWin::onMouseDown(MouseEvent event)
{
    if(WidgetWindow::onMouseDown(event))
        return true;
    for(auto f : files) {
        if(f->onMouseDown(event)) {
            return true;
        }
    }
    return false;
}

bool FileViewerWin::onMouseDrag(MouseEvent event)
{
    if(WidgetWindow::onMouseDrag(event))
        return true;
    for(auto f : files) {
        if(f->onMouseDrag(event)) {
            return true;
        }
    }
    return false;
}

bool FileViewerWin::onMouseUp(MouseEvent event)
{
    if(WidgetWindow::onMouseUp(event))
        return true;
    for(auto f : files) {
        if(f->onMouseUp(event)) {
            return true;
        }
    }
    return false;
}

bool FileViewerWin::onAccept(MouseEvent event, shared_ptr<Entity> e)
{
    ivec2 pos = event.getPos() - getPos2D();
    shared_ptr<File> f = shared_ptr<File>(new File(this, e->getName(), "geos-kernal.png", e.get()));
    f->setThumbnail(e->getShadow()->fbo->getColorTexture(), e->getShadow()->texcos);
    f->setPos(pos);
    addFile(f);
    return true;
}

FileViewer::FileViewer()
:PRGLauncher("file viewer")
{
}

void FileViewer::launch()
{
    game::guiMgr.addWidget(new FileViewerWin(name));
}

bool FileViewer::onMouseDown(MouseEvent event)
{
    return PRGLauncher::onMouseDown(event);
}

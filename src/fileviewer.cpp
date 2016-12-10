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
    if(!contains(event.getPos()))
       return false;
    
    grab(event.getPos());
    
    if(parent != nullptr) {
        detached = parent;
        detachedPos = getPos2D();
    }
    reparent(nullptr);
    return true;
    //entity->generateShadow();
}

bool File::onMouseUp(MouseEvent event)
{
    if(!grabbed)
        return false;
    
    if(entity != nullptr) {
        entity->setEnabled(true);
        entity->setPos(game::screenToWorld(event.getPos(), 5.0f));
        game::guiMgr.onAccept(event, entity);
        
        if(parent != nullptr) {
            parent->removeChild(this);
        } else {
            game::guiMgr.remove(this);
        }
    } else if(detached != nullptr) {
        // no entity, snap file back to its parent
        reparent(detached);
        setPos(detachedPos);
    }
    
    ungrab();
    return true;
}

bool File::onMouseDrag(MouseEvent event)
{
    if(!grabbed)
        return false;
    
    if(entity != nullptr && !entity->isGrabbed()) {
        entity->setEnabled(false);
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
    gl::drawString(name, ivec2(0, dim.y));
}


bool SpellFile::onMouseUp(MouseEvent event)
{
    if(!grabbed)
        return false;

    shared_ptr<Entity> e = game::getPicked(event.getPos());
    activate(e);
    return false;
}

FileViewerWin::FileViewerWin(string name)
:PRG(name)
{
    //shared_ptr<File> f(new File(nullptr, "test", "geos-kernal.png"));
    shared_ptr<File> f(new SpellFile(nullptr, "ZAP", "", shared_ptr<Spell>(new Zap()), nullptr));
    addChild(f);
}

void FileViewerWin::addFile(shared_ptr<File> f)
{
    addChild(f);
}

void FileViewerWin::draw()
{
    WidgetWindow::draw();
    if(!show)
        return;
    
    for(auto f : children) {
        f->draw();
        f->apply();
    }
}

bool FileViewerWin::onMouseDown(MouseEvent event)
{
    if(WidgetWindow::onMouseDown(event))
        return true;
    for(auto f : children) {
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
    for(auto f : children) {
        if(f->onMouseDrag(event)) {
            return true;
        }
    }
    return false;
}

bool FileViewerWin::onAccept(MouseEvent event, shared_ptr<Entity> e)
{
    ivec2 pos = event.getPos() - getPos2D() + getScroll();
    shared_ptr<File> f = shared_ptr<File>(new File(nullptr, e->getName(), "geos-kernal.png", e));
    f->setThumbnail(e->getShadow()->fbo->getColorTexture(), e->getShadow()->texcos);
    f->setPos(pos);
    addFile(f);
    
    if(e != nullptr)
        e->setEnabled(false);
    
    return true;
}

FileViewer::FileViewer(string name)
:PRGLauncher(name)
{
}

void FileViewer::launch()
{
    game::guiMgr.addWidget(shared_ptr<FileViewerWin>(new FileViewerWin(name)));
}

bool FileViewer::onMouseDown(MouseEvent event)
{
    return PRGLauncher::onMouseDown(event);
}

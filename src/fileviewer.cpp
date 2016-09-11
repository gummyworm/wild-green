//
//  fileviewer.cpp
//  digital
//
//  Created by Bryce on 8/29/16.
//
//

#include "fileviewer.hpp"
#include "entity.hpp"
#include "properties.h"

FileViewerWin::FileViewerWin(string name)
:PRG(name)
{
    shared_ptr<File> f(new File("test", "geos-kernal.png"));
    addFile(f);
}

void FileViewerWin::addFile(shared_ptr<File> f)
{
    files.push_back(f);
}

void FileViewerWin::draw()
{
    if(!show)
        return;
    WidgetWindow::draw();
    
    gl::pushMatrices();
    gl::setMatricesWindow(properties::screenSize);
    
    for(auto f : files) {
        gl::draw(f->getThumbnail(), f->getPos() + ivec2(getInternalRect().getUpperLeft()));
    }
    
    gl::popMatrices();
}

void FileViewerWin::onMouseDown(MouseEvent event)
{
    WidgetWindow::onMouseDown(event);
    
    ivec2 mousePos = event.getPos();
    ivec2 winOffset = getInternalRect().getUpperLeft();
    for(auto f : files) {
        Rectf r(f->getPos() + winOffset, f->getPos() + thumbnailSize + winOffset);
        if(r.contains(mousePos)) {
            selected = f;
            cout << selected->getName() << endl;
            break;
        }
    }
}

void FileViewerWin::onAccept(MouseEvent event, shared_ptr<Entity> e)
{
    ivec2 pos = event.getPos() - ivec2(getInternalRect().getUpperLeft());
    addFile(shared_ptr<File>(new File(e->getName(), "geos-kernal.png", nullptr, pos)));
}

FileViewer::FileViewer()
:PRGLauncher("file viewer")
{
}

void FileViewer::launch()
{
    PRGLauncher::launch(shared_ptr<FileViewerWin>(new FileViewerWin(name)));
}

void FileViewer::onMouseDown(MouseEvent event)
{
    PRGLauncher::onMouseDown(event);
    if(getRect().contains(event.getPos()))
        launch();
}

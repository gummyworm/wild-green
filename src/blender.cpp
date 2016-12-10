//
//  blender.cpp
//  digital
//
//  Created by Bryce on 11/28/16.
//
//

#include "blender.hpp"
#include "game.h"

BlenderWin::BlenderWin(string name)
:FileViewerWin(name)
{
    shared_ptr<TextButton> btn(new TextButton(nullptr, "MIX"));
    btn->setPos(ivec2(100,50));
    addChild(btn);
}

Blender::Blender()
:FileViewer("Blender")
{
}

void Blender::launch()
{
    game::guiMgr.addWidget(shared_ptr<BlenderWin>(new BlenderWin(name)));
}
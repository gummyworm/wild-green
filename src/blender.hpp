//
//  blender.hpp
//  digital
//
//  Created by Bryce on 11/28/16.
//
//

#ifndef blender_hpp
#define blender_hpp

#include <stdio.h>
#include "fileviewer.hpp"
#include "button.hpp"

class BlenderWin : public FileViewerWin {
public:
    BlenderWin(string name="");
};


class Blender : public FileViewer {
public:
    Blender();
    void launch() override;
};

#endif /* blender_hpp */

//
//  menubar.hpp
//  digital
//
//  Created by Bryce on 8/27/16.
//
//

#ifndef menubar_hpp
#define menubar_hpp

#include <functional>
#include <map>
#include <stdio.h>
#include <string>
#include <vector>
#include "menu.hpp"
#include "widget.hpp"
#include "contextmenu.hpp"
#include <cinder/gl/TextureFont.h>

using namespace std;
using namespace ci;
using namespace ci::app;

class MenuBar : public Widget {
protected:
    vector<shared_ptr<ContextMenu>> menus;
    
    int selectedMenu;
    int height;
public:
    MenuBar():Widget(),selectedMenu(-1), height(16){
        pos = vec3(0,0,0);
        flags.drawBorder = false;
    }
    
    void addSubmenu(shared_ptr<ContextMenu> m);
    void draw() override;
    bool onMouseDrag(MouseEvent event) override;
    bool onMouseDown(MouseEvent event) override;
    bool onMouseUp(MouseEvent event) override;
};

#endif /* menubar_hpp */

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

using namespace std;
using namespace ci;
using namespace ci::app;

class MenuBar : public Widget {
protected:
    vector<Menu> menus;
    
    int selectedMenu;
    int height;
public:
    MenuBar():Widget(),selectedMenu(-1), height(16){pos = vec3(0,0,0);}
    
    void addSubmenu(Menu m);
    void addItem(string submenu, string name, function<void()> callback);
    void draw() override;
    bool onMouseDrag(MouseEvent event) override;
    bool onMouseDown(MouseEvent event) override;
    bool onMouseUp(MouseEvent event) override;
};

#endif /* menubar_hpp */

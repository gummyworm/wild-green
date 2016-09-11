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
#include "widget.hpp"

using namespace std;
using namespace ci;
using namespace ci::app;

class MenuBar : public Widget {
protected:
    struct MenuItem {
    public:
        string name;
        function<void()> onActivate;
        
        MenuItem(string name, function<void()> callback):name(name), onActivate(callback) {}
    };
    
    struct Submenu {
    public:
        bool open;
        string name;
        Rectf rect;
        vector<MenuItem> items;
        int selectedItem;
        
        Submenu(string name):name(name), open(false) {}
        void addItem(string name, function<void()> callback) {items.push_back(MenuItem(name, callback));}
        void draw();
    };
    
    vector<Submenu> menus;
    
    int selectedMenu;
    int height;
public:
    MenuBar():Widget(),selectedMenu(-1), height(16) {}
    
    void addSubmenu(string name);
    void addItem(string submenu, string name, function<void()> callback);
    void draw() override;
    void onMouseDrag(MouseEvent event) override;
    void onMouseDown(MouseEvent event) override;
    void onMouseUp(MouseEvent event) override;
};

#endif /* menubar_hpp */

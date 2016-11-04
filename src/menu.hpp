//
//  menuitem.hpp
//  digital
//
//  Created by Bryce on 9/20/16.
//
//

#ifndef menuitem_hpp
#define menuitem_hpp

#include <stdio.h>
#include "widget.hpp"

using namespace std;

class MenuItem {
    string name;
    function<void()> action;
    shared_ptr<class ContextMenu> submenu;
public:
    MenuItem(string name=""):submenu(nullptr) {this->name = name;}
    MenuItem(string name, function<void()> onSelect):MenuItem(name) {action = onSelect;}
    MenuItem(string name, shared_ptr<class ContextMenu> submenu):MenuItem(name) {this->submenu = submenu;}
    
    string getName() {return name;}
    function<void()> getAction() {return action;}
    shared_ptr<class ContextMenu> getSubmenu() {return submenu;}
};

class Menu : public Widget {
public:
    bool open;
    string name;
    Rectf rect;
    vector<MenuItem> items;
    int selectedItem;
    
    Menu(string name):name(name), open(false) {}
    void addItem(string name, function<void()> callback) {items.push_back(MenuItem(name, callback));}
};

#endif /* menuitem_hpp */

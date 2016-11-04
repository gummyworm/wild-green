//
//  contextmenu.hpp
//  digital
//
//  Created by Bryce on 8/23/16.
//
//

#ifndef contextmenu_hpp
#define contextmenu_hpp

#include <string>
#include "menu.hpp"

using namespace std;

class ContextMenu : public Widget {
protected:
    vector<MenuItem> items;
    ColorA hoverColor;
    ColorA highlightColor;
    int highlightedItem;
    
    int itemOffset(int);
public:
    ContextMenu();
    void addItem(string name, function<void()> callback);
    void addItem(string name, shared_ptr<ContextMenu> submenu);
    void draw() override;
    
    void onMouseDown(MouseEvent event) override;
    void onMouseUp(MouseEvent event) override;
    void onMouseMove(MouseEvent event) override;
    void onMouseDrag(MouseEvent event) override;
};

#endif /* contextmenu_hpp */

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
#include <cinder/gl/TextureFont.h>

using namespace std;

class ContextMenu : public Widget {
protected:
    string name;
    vector<MenuItem> items;
    ColorA hoverColor;
    ColorA highlightColor;
    int highlightedItem;
    gl::TextureFontRef font;
    
    int itemOffset(int);
    
    bool fixedPos;
public:
    ContextMenu(string name="");
    void addItem(string name, function<void()> callback);
    void addItem(string name, shared_ptr<ContextMenu> submenu);
    void draw() override;
    
    bool onMouseDown(MouseEvent event) override;
    bool onMouseUp(MouseEvent event) override;
    bool onMouseMove(MouseEvent event) override;
    bool onMouseDrag(MouseEvent event) override;
    
    string getName() {return name;}
    void setFixed(bool fix) {fixedPos = fix;}
};

#endif /* contextmenu_hpp */

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
#include "widget.hpp"

using namespace std;

class ContextMenu : public Widget {
    vector<pair<string, function<void()>>> actions;
    ColorA hoverColor;
    ColorA highlightColor;
    int highlightedItem;
    
    int itemOffset(int);
public:
    ContextMenu();
    void addItem(string name, function<void()> callback);
    void draw() override;
    
    void onMouseDown(MouseEvent event) override;
    void onMouseUp(MouseEvent event) override;
    void onMouseMove(MouseEvent event) override;
    void onMouseDrag(MouseEvent event) override;

};

#endif /* contextmenu_hpp */

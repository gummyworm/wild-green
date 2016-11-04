//
//  guimanager.hpp
//  digital
//
//  Created by Bryce on 11/2/16.
//
//

#ifndef guimanager_hpp
#define guimanager_hpp

#include <stdio.h>
#include "widget.hpp"

class GUIManager {
protected:
    vector<Widget*> widgets;
    vector<Widget*> newWidgets;
public:
    GUIManager();
    
    void addWidget(Widget *w);
    
    void bringFront(Widget *widget);
    void moveBack(Widget *widget);
    
    void onMouseDown(MouseEvent event);
    void onMouseUp(MouseEvent event);
    void onMouseMove(MouseEvent event);
    void onMouseDrag(MouseEvent event);
    void onKeydown(KeyEvent event);
    void onAccept(MouseEvent event, shared_ptr<class Entity> e);
    void update();
    void draw();
};


#endif /* guimanager_hpp */

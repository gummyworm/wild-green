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
    vector<shared_ptr<Widget>> widgets;
    vector<shared_ptr<Widget>> newWidgets;
    vector<Widget*> toRemove;
public:
    GUIManager();
    
    void addWidget(shared_ptr<Widget> w);
    void remove(Widget *w);
    shared_ptr<Widget> getWidget(Widget *w);
    
    void bringFront(Widget *widget);
    void moveBack(Widget *widget);
    
    void onMouseDown(MouseEvent event);
    void onMouseUp(MouseEvent event);
    void onMouseMove(MouseEvent event);
    void onMouseDrag(MouseEvent event);
    bool onKeydown(KeyEvent event);
    
    bool onAccept(MouseEvent event, shared_ptr<class Entity> e);
    bool onAccept(MouseEvent event, shared_ptr<Widget> w);
    
    void update();
    void draw();
};


#endif /* guimanager_hpp */

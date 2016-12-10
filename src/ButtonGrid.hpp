//
//  ButtonGrid.hpp
//  digital
//
//  Created by Bryce on 11/10/16.
//
//

#ifndef ButtonGrid_hpp
#define ButtonGrid_hpp

#include <stdio.h>

#include "cinder/gl/gl.h"
#include "widget.hpp"
#include "button.hpp"

using namespace cinder;

class ButtonGrid : public Widget {
protected:
    ColorA pressedColor;
    ColorA releasedColor;
    
    shared_ptr<Widget> selected;
    int selectedIndex;
    
    int rows, cols;
public:
    ButtonGrid(Widget *parent, int w, int h, vector<shared_ptr<Button>> buttons=vector<shared_ptr<Button>>());
    bool onMouseDown(MouseEvent event) override;
    bool onMouseUp(MouseEvent event) override;
    void layout();
    void draw() override;
    
    int getSelectedIndex() {return selectedIndex;}
    shared_ptr<Widget> getSelected() {return selected;}
};

#endif /* ButtonGrid_hpp */

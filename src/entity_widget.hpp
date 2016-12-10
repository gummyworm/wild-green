//
//  entity_widget.hpp
//  digital
//
//  Created by Bryce on 12/5/16.
//
//

#ifndef entity_widget_hpp
#define entity_widget_hpp

#include <stdio.h>
#include <cinder/gl/Texture.h>
#include "entity.hpp"
#include "widget.hpp"

// EntityWidget is a class used to tie an entity reference to a draggable widget.
// It contains functionality for communicating between the entity, the GUI and the 3D world.
class EntityWidget : public Widget {
protected:
    shared_ptr<Entity> entity;
    gl::TextureRef icon;
    bool destroyOnMouseUp;
    bool colorize;
public:
    EntityWidget(shared_ptr<Entity> e, gl::TextureRef icon, bool destroyOnMouseUp=true, bool colorize=true)
    :entity(e),
    icon(icon),
    destroyOnMouseUp(destroyOnMouseUp) {
        flags.drawBorder = false;
        flags.drawBg = false;
    }
    
    bool onMouseDown(MouseEvent event) override;
    bool onMouseUp(MouseEvent event) override;
    bool onMouseDrag(MouseEvent event) override;
    void draw() override;
};

#endif /* entity_widget_hpp */
//
//  verticallayout.hpp
//  digital
//
//  Created by Bryce on 11/19/16.
//
//

#ifndef verticallayout_hpp
#define verticallayout_hpp

#include <stdio.h>
#include "widget.hpp"

class VerticalLayout : Widget {
    ivec2 cursor;
public:
    VerticalLayout();
    void addChild(shared_ptr<Widget> child) override;
};

#endif /* verticallayout_hpp */

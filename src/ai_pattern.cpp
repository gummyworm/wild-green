//
//  ai_pattern.cpp
//  digital
//
//  Created by Bryce on 10/10/16.
//
//

#include "ai_pattern.hpp"

AIPattern::AIPattern(Entity *entity, const fs::path iconFile)
:entity(entity)
{
    if(!iconFile.empty())
        icon = gl::Texture::create(loadImage(loadAsset(iconFile)));
}

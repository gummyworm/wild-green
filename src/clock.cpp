//
//  clock.cpp
//  digital
//
//  Created by Bryce on 9/27/16.
//
//

#include "clock.hpp"
#include "properties.h"

Clock::Clock()
:Entity("clock")
{
    auto rect = geom::Rect().texCoords(vec2(0,0), vec2(1,0), vec2(0,1), vec2(1,1));
    auto shader = gl::ShaderDef().texture().lambert();
    batch = gl::Batch::create(rect, gl::getStockShader(shader));
    scale = vec3(1.0f,0.5f,1.0f);
    
    timeFbo = gl::Fbo::create(512, 64);
    font = gl::TextureFont::create(properties::clockFont);
}

void Clock::setMessage(string msg)
{
    
}

void Clock::draw()
{
    timeFbo->bindFramebuffer();
    timeFbo->unbindFramebuffer();
    timeFbo->getColorTexture();
}
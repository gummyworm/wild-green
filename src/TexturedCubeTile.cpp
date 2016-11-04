//
//  TexturedCubeTile.cpp
//  digital
//
//  Created by Bryce on 10/24/16.
//
//

#include "TexturedCubeTile.hpp"

using namespace cinder;
using namespace cinder::app;

TexturedCubeTile::TexturedCubeTile(const fs::path texture)
:MapTile()
{
    tex = gl::Texture::create(loadImage(loadAsset(texture)));
    tex->bind();

    auto cube = geom::Cube();
    auto shader = gl::ShaderDef().texture().lambert();
    batch = gl::Batch::create(cube, gl::getStockShader(shader));
    
    tex->unbind();
}

void TexturedCubeTile::draw()
{
    gl::enable(GL_TEXTURE_2D);
    gl::enable(GL_TEXTURE0);
    tex->bind();

    batch->draw();
    tex->unbind();
}
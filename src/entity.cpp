//
//  entity.cpp
//  pool
//
//  Created by Bryce on 8/21/16.
//
//

#include "entity.hpp"
#include "cinder/ObjLoader.h"
#include "properties.h"
#include "game.h"
#include "cinder/AxisAlignedBox.h"

using namespace ci;
using namespace cinder;
using namespace ci::app;


void Entity::setupShaders()
{
    shaders.normal = gl::GlslProg::create(loadAsset("postpass.vert"), loadAsset("postpass.frag"));
    shaders.shaded = gl::GlslProg::create(loadAsset("postpass.vert"), loadAsset("postpass.frag"));
    shaders.outline = gl::GlslProg::create(loadAsset("postpass.vert"), loadAsset("postpass.frag"));
    shaders.ui = gl::getStockShader(gl::ShaderDef().lambert().color());
    shaders.shadow = gl::getStockShader(gl::ShaderDef().lambert().texture());
}

Entity::Entity(string name, const fs::path &model, gl::GlslProgRef prog, const fs::path &thumbnail)
:name(name),
pos(vec3(0)),
scale(vec3(1)),
rotation(vec3(0)),
memoryUse(64),
hp(100),
maxHp(100),
highlightColor(properties::highlightColor),
speed(1.0f),
thumbnail(nullptr)
{
    flags.enabled = true;
    flags.hasGravity = true;
    flags.grabbable = true;
    flags.solid = true;
    flags.movable = true;
    flags.highlight = false;
    flags.visible = true;
    
    if(prog == nullptr)
        prog = gl::getStockShader(gl::ShaderDef().lambert().color());
    
    if(model.empty()) {
        mesh = TriMesh::create(geom::Cube());
        batch = gl::Batch::create(*mesh, prog);
        aabb = mesh->calcBoundingBox();
    } else {
        ObjLoader ol(loadAsset(model));
        mesh = TriMesh::create(ol);
        aabb = mesh->calcBoundingBox();
        batch = gl::Batch::create(ol, gl::getStockShader(gl::ShaderDef().lambert().color()));
    }
    
    if(!thumbnail.empty()) {
        this->thumbnail = gl::Texture::create(loadImage(loadAsset(thumbnail)));
    }
    
    actions = shared_ptr<ContextMenu>(new ContextMenu());
    
    setupShaders();
    shadow.fbo = gl::Fbo::create(properties::screenWidth, properties::screenHeight);
    font = gl::TextureFont::create(Font(properties::entityLabelFont));
}



void Entity::render(Camera cam)
{
    //glm::rotate(rotation.x, vec3(1, 0, 0));
    //glm::rotate(rotation.y, vec3(0, 1, 0));
    //glm::rotate(rotation.z, vec3(0, 0, 1));
    
    gl::pushModelMatrix();
    gl::setMatrices(cam);

    //gl::setModelMatrix(transform);
    gl::translate(pos);
    gl::scale(scale);
    
    if(flags.highlight)
        gl::color(highlightColor);
    else
        gl::color(ColorA(1,0,0,1));
    batch->draw();
    for(auto e : children)
        e->draw(cam);
    gl::popModelMatrix();
}

void Entity::draw(Camera cam)
{
    if(!flags.visible)
        return;
    render(cam);
    if(shadow.draw) {
        gl::setMatricesWindow(properties::screenSize);
        shaders.shadow->bind();
        shaders.shadow->uniform("uTex0", 0);
        shadow.fbo->bindTexture(0);
        
        Rectf r(shadow.pos, shadow.pos + shadow.dim);
        gl::enableAlphaBlending();
        gl::enable(GL_TEXTURE_2D);
        gl::drawSolidRect(r,
                          vec2(shadow.texcos.x1, shadow.texcos.y1),
                          vec2(shadow.texcos.x2, shadow.texcos.y2));
        shadow.fbo->unbindTexture();
        gl::disable(GL_TEXTURE_2D);
    }
}

void Entity::onResize()
{
    shadow.fbo = gl::Fbo::create(properties::screenWidth, properties::screenHeight);
}

void Entity::label(Camera cam)
{
    if(cam.worldToEyeDepth(pos) > 0)
        return;
    
    gl::pushModelMatrix();
    gl::setMatricesWindow(properties::screenSize);
    
    string memStr = to_string(memoryUse) + "B";
    
    vec2 size1 = font->measureString(name);
    vec2 size2 = font->measureString(memStr);
    
    vec2 line1 = cam.worldToScreen(aabb.getMax()*scale + pos, properties::screenWidth, properties::screenHeight) - vec2(0, size1.y+size2.y);
    
    vec2 line2 = line1 + vec2(0, size1.y);
    Rectf rect(line1.x, line1.y-size1.y, line1.x+size1.x, line2.y);
    
    shaders.ui->bind();
    
    gl::color(properties::speechboxBorderColor);
    gl::drawStrokedRect(rect, properties::speechboxBorderWidth);
    gl::color(properties::labelColor);
    //gl::drawSolidRect(rect);
    gl::drawSolidRoundedRect(rect, 0);
    gl::color(properties::labelTextColor);
    font->drawString(name, line1);
    font->drawString(memStr, line2);
    for(auto e : children)
        e->label(cam);
    
    gl::popModelMatrix();
}

void Entity::say(string text)
{
    game::say(this, text);
}

void Entity::addAction(string name, function<void()> fn)
{
    actions->addItem(name, fn);
}

void Entity::setDrawState(EntityDrawState state)
{
    drawState = state;
}

void Entity::onMouseDown(MouseEvent event, Camera cam)
{
    generateShadow();
    shadow.grabbedOffset = shadow.pos - event.getPos();
}

void Entity::generateShadow()
{
    auto aabb = this->aabb.transformed(transform);
    vec3 min = aabb.getMin();
    vec3 max = aabb.getMax();
    vec3 pts[] = {
        vec3(min.x, min.y, min.z),
        vec3(min.x, min.y, max.z),
        vec3(min.x, max.y, min.z),
        vec3(min.x, max.y, max.z),
        vec3(max.x, min.y, min.z),
        vec3(max.x, min.y, max.z),
        vec3(max.x, max.y, min.z),
        vec3(max.x, max.y, max.z)
    };

    vec2 screenMin(INFINITY, INFINITY);
    vec2 screenMax(-INFINITY, -INFINITY);
    
    for(auto p : pts) {
        vec2 xy = mainCam.worldToScreen(p, properties::screenWidth, properties::screenHeight); //getWindowWidth(), getWindowHeight());
        if(xy.x < screenMin.x)
            screenMin.x = xy.x;
        if(xy.y < screenMin.y)
            screenMin.y = xy.y;
        
        if(xy.x > screenMax.x)
            screenMax.x = xy.x;
        if(xy.y > screenMax.y)
            screenMax.y = xy.y;
    }
    
    shadow.pos = ivec2(screenMin.x, screenMin.y);
    shadow.dim = ivec2(screenMax.x - screenMin.x, screenMax.y - screenMin.y);
    
    //render the "shadow" to be dragged
    shadow.fbo->bindFramebuffer();
    
    shadow.texcos.x1 = screenMin.x / properties::screenWidth;
    shadow.texcos.y1 = 1.0f - (screenMin.y / properties::screenHeight);
    shadow.texcos.x2 = screenMax.x / properties::screenWidth;
    shadow.texcos.y2 = 1.0f - (screenMax.y / properties::screenHeight);
    
    gl::clear(ColorA(0,0,0,0));
    gl::disableDepthRead();
    gl::disableDepthWrite();
    render(mainCam);
    shadow.fbo->unbindFramebuffer();
    
    shadow.grabbedOffset = ivec2();
    shadow.draw = true;
}

void Entity::onMouseMove(MouseEvent event)
{
    
}

void Entity::onMouseDragged(MouseEvent event)
{
    if(!shadow.draw)
        return;
    shadow.pos = event.getPos() + shadow.grabbedOffset;
}

void Entity::onMouseUp(MouseEvent event)
{
    shadow.draw = false;
}

void Entity::update()
{
    transform = mat4(1.0f);
    transform *= glm::translate(pos);
    transform *= glm::scale(scale);

    if(!flags.hasGravity || flags.grounded)
        return;
    if(pos.y > 0)
        pos.y -= FALL_SPEED;
    if(pos.y < 0)
        pos.y = 0;
}
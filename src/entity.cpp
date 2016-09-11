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

Entity::Entity(string name, const fs::path &model, const fs::path &vert, const fs::path &frag)
:name(name),
pos(vec3(0)),
scale(vec3(1)),
rotation(vec3(0)),
memoryUse(64),
hp(100),
maxHp(100)
{
    gl::GlslProgRef shader;
    
    if(vert.empty() || frag.empty()) {
        shader = gl::getStockShader(gl::ShaderDef().lambert().color());
    } else {
        shader = gl::GlslProg::create(loadAsset(vert), loadAsset(frag));
    }
 
    if(model.empty()) {
        mesh = geom::Cube();
        batch = gl::Batch::create(mesh, shader);
    } else {
        mesh.create(ObjLoader(loadAsset(model)));
        batch = gl::Batch::create(mesh, shader);
    }

    setupShaders();
    shadow.fbo = gl::Fbo::create(properties::screenWidth, properties::screenHeight);
    scale = vec3(.1f);
}

void Entity::render(Camera cam)
{
    //glm::rotate(rotation.x, vec3(1, 0, 0));
    //glm::rotate(rotation.y, vec3(0, 1, 0));
    //glm::rotate(rotation.z, vec3(0, 0, 1));

    gl::pushModelMatrix();
    gl::setMatrices(cam);
    gl::setModelMatrix(transform);

    batch->draw();
    for(auto e : children)
        e->draw(cam);
    gl::popModelMatrix();
}

void Entity::draw(Camera cam)
{
    render(cam);
    gl::color(ColorA(1,0,0,1));
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
    }
}

void Entity::onResize()
{
    shadow.fbo = gl::Fbo::create(properties::screenWidth, properties::screenHeight);
}

void Entity::label(Camera cam)
{
    if(!shadow.draw)
        return;
    
    vec2 scr = cam.worldToScreen(pos*scale, properties::screenWidth, properties::screenHeight);
    scr.y -= 64;
    
    gl::pushModelMatrix();
    gl::setMatricesWindow(properties::screenSize);
    
    shaders.ui->bind();
    gl::drawString(name + to_string(memoryUse), scr, ColorA(1,0.2f,0.4f,1.0f), Font("Arial", 32));
    for(auto e : children)
        e->label(cam);
    
    gl::popModelMatrix();
}

void Entity::addAction(string name, function<void()> fn)
{
    actions.addItem(name, fn);
}

void Entity::setDrawState(EntityDrawState state)
{
    drawState = state;
}

void Entity::onMouseDown(MouseEvent event, Camera cam)
{
    AxisAlignedBox aabb = mesh.calcBoundingBox(transform);
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
        vec2 xy = cam.worldToScreen(p, properties::screenWidth, properties::screenHeight); //getWindowWidth(), getWindowHeight());
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
    gl::enableDepthWrite();
    gl::enableDepthRead();
    shadow.fbo->bindFramebuffer();
    
    shadow.texcos.x1 = screenMin.x / properties::screenWidth;
    shadow.texcos.y1 = 1.0f - (screenMin.y / properties::screenHeight);
    shadow.texcos.x2 = screenMax.x / properties::screenWidth;
    shadow.texcos.y2 = 1.0f - (screenMax.y / properties::screenHeight);
    
    cout << vec2(shadow.texcos.x1, shadow.texcos.y1) << "  MAX: " << vec2(shadow.texcos.x2, shadow.texcos.y2) << endl;

    shadow.grabbedOffset = screenMin - vec2(event.getPos());
    
    gl::clear(ColorA(0,0,0,0));
    render(cam);
    gl::disableDepthRead();
    gl::disableDepthWrite();

    shadow.fbo->unbindFramebuffer();
    shadow.draw = true;
}

void Entity::onMouseDragged(MouseEvent event)
{
    if(!shadow.draw)
        return;
    shadow.pos = event.getPos() + shadow.grabbedOffset;
}

void Entity::onMouseUp(MouseEvent event)
{
    if(shadow.draw == false)
        return;
    
    gl::setMatricesWindow(properties::screenWidth, properties::screenHeight);
    //mat4 proj = gl::context()->getProjectionMatrixStack().back() * gl::context()->getViewMatrixStack().back();
    mat4 proj = mainCam.getProjectionMatrix();
    vec4 viewport = vec4(0, 0, properties::screenWidth, -properties::screenHeight);
    
    float x = event.getX();
    float y = properties::screenHeight - event.getY();
    vec3 p = screenToWorld(event.getPos(), 0.0f);
    
    cout << p << endl;
    setPos(p);
    shadow.draw = false;
}

void Entity::update()
{
    //if(!flags.hasGravity || flags.grounded)
        //return;
    transform = mat4(1.0f);
    transform *= glm::scale(scale);
    transform *= glm::translate(pos);

    if(pos.y > 0)
        pos.y -= FALL_SPEED;
    if(pos.y < 0)
        pos.y = 0;
}
//
//  game.cpp
//  digital
//
//  Created by Bryce on 9/6/16.
//
//

#include "cinder/gl/Fbo.h"
#include "conversation.hpp"
#include "entity.hpp"
#include "party.hpp"
#include "contextmenu.hpp"
#include "window.hpp"
#include "inspector.hpp"
#include "menubar.hpp"
#include "playercam.hpp"
#include "fileviewer.hpp"
#include "combat.hpp"
#include "properties.h"
#include "game.h"

// rendering
PlayerCam mainCam;
static gl::GlslProgRef postPass;
static gl::GlslProgRef uiProg;
static gl::FboRef fbo;
static gl::FboRef pixelateFbo;

// input
static bool uiFocused; // if true, UI events are handled
static vec2 mousePos;
static shared_ptr<Entity> grabbed;  // the entity that the client is "holding"

// UI elements
static MenuBar menuBar;
static Party party;
static ContextMenu *menu;
static Conversation convo;

// game state
static vector<shared_ptr<Widget>> windows;
static vector<shared_ptr<Entity>> entities;
static vector<TriMesh*> pickable;

static bool pick(Entity *e, vec3 *pickedPoint, vec3 *pickedNormal)
{
    TriMesh *mesh = e->getMesh();
    mat4 transform = e->getTransform();
    
    // Generate a ray from the camera into our world. Note that we have to
    // flip the vertical coordinate.
    float u = mousePos.x / (float)properties::screenWidth;
    float v = mousePos.y / (float)properties::screenHeight;
    Ray ray = mainCam.generateRay(u, 1.0f-v, properties::aspectRatio);
    
    AxisAlignedBox worldBoundsExact = mesh->calcBoundingBox(transform); // slow
    
    if(!worldBoundsExact.intersects(ray))
        return false;
    
    // Set initial distance to something far, far away.
    float result = FLT_MAX;
    
    // Traverse triangle list and find the closest intersecting triangle.
    const size_t polycount = mesh->getNumTriangles();
    float distance = 0.0f;
    for(size_t i = 0; i < polycount; ++i ) {
        // Get a single triangle from the mesh.
        vec3 v0, v1, v2;
        mesh->getTriangleVertices(i, &v0, &v1, &v2);
        
        // Transform triangle to world space.
        v0 = vec3(transform * vec4(v0, 1.0));
        v1 = vec3(transform * vec4(v1, 1.0));
        v2 = vec3(transform * vec4(v2, 1.0));
        
        // Test to see if the ray intersects this triangle.
        if(ray.calcTriangleIntersection(v0, v1, v2, &distance)) {
            // Keep the result if it's closer than any intersection we've had so far.
            if(distance < result) {
                result = distance;
            }
        }
    }
    
    // Did we have a hit?
    if( distance > 0 ) {
        // Calculate the exact position of the hit.
        *pickedPoint = ray.calcPosition(result);
        return true;
    } else {
        return false;
    }
}


void test()
{
    
    unique_ptr<PartyMember> camel(new PartyMember("camel", "camel.jpg"));
    unique_ptr<PartyMember> camel2(new PartyMember("camel", "camel.jpg"));
    party.addMember(move(camel));
    party.addMember(move(camel2));
    
    //camel->joinConversation(&convo);
    //camel->say("HELLO!");
    
    shared_ptr<Entity> testE(new Entity("teapot"));
    testE->setPos(vec3(0,3.0f,0));
    
    shared_ptr<FileViewer> fileViewer(new FileViewer());
    windows.push_back(fileViewer);
    
    shared_ptr<CombatLauncher> combatWin(new CombatLauncher(testE));
    windows.push_back(combatWin);
    
    testE->addAction("OPEN WITH FILE VIEWER", [fileViewer]() mutable {
        fileViewer->launch();
    });
    testE->addAction("FIGHT", [combatWin]() mutable {
        combatWin->launch();
    });
    testE->addAction("test action3", [](){});
    testE->addAction("test action4", [](){});
    
    entities.push_back(testE);
    
    // setup the menu
    menuBar.addSubmenu("FILE");
    menuBar.addSubmenu("EDIT");
    
    shared_ptr<Inspector> win(new Inspector(gl::Batch::create(geom::Cube(), gl::getStockShader(gl::ShaderDef().lambert().color()))));
    windows.push_back(win);

}

void game::setup()
{
    uiProg = gl::getStockShader(gl::ShaderDef().lambert().color());
    postPass = gl::GlslProg::create(loadAsset("postpass.vert"), loadAsset("postpass.frag"));
    fbo = gl::Fbo::create(getWindowWidth(), getWindowHeight());
    pixelateFbo = gl::Fbo::create(properties::screenWidth, properties::screenHeight);
    
    test();
}

void game::start()
{
    
}

void game::draw()
{
    gl::enableDepthWrite();
    gl::enableDepthRead();
    
    fbo->bindFramebuffer();
    gl::clear(Color(0.9f,0.9f,0.8f));
    
    // draw the scene
    gl::setMatrices(mainCam);
    
    gl::color(1.0f, 1.0f, 1.0f);
    gl::drawCube(vec3(-2, 0, 0), vec3(2,2,2));
    gl::color(0.7f, 0.3f, 0.65f);
    
    for(auto e : entities)
        e->draw(mainCam);
    
    fbo->unbindFramebuffer();
    
    pixelateFbo->bindFramebuffer();
    gl::viewport(ivec2(0), properties::screenSize);
    gl::clear(ColorA(0,0,0,1));
    gl::setMatricesWindow(properties::screenSize);
    postPass->bind();
    postPass->uniform("uTex0", 0);
    fbo->bindTexture(0);
    gl::enable(GL_TEXTURE_2D);
    gl::drawSolidRect(pixelateFbo->getBounds());
    fbo->unbindTexture();
    pixelateFbo->unbindFramebuffer();
    
    gl::clear(Color(1,1,1));
    gl::color(Color(1,1,1));
    gl::viewport(0, 0, getWindowWidth(), getWindowHeight());
    auto tex = pixelateFbo->getColorTexture();
    tex->setMinFilter(GL_NEAREST);
    tex->setMagFilter(GL_NEAREST);
    tex->bind(0);
    gl::draw(tex, tex->getBounds()); //Rectf(tex->getBounds()).getCenteredFit(getWindowBounds(), true));
    
    // draw the UI
    gl::disableDepthRead();
    gl::disableDepthWrite();
    uiProg->bind();
    
    for(auto e : entities) {
        e->label(mainCam);
    }
    
    gl::color(1,1,1);
    gl::setMatricesWindow(properties::screenSize);
    party.draw();
    menuBar.draw();
    
    if(menu != nullptr)
        menu->draw();
    
    for(auto w : windows)
        w->draw();
}

void game::resize(ivec2 size)
{
    mainCam.onResize();
    fbo = gl::Fbo::create(size.x, size.y);
    for(auto e : entities) {
        e->onResize();
    }
}

void game::update()
{
    for(auto e : entities)
        e->update();
}

void game::onMouseDown(MouseEvent event)
{
    mousePos = event.getPos();
    menu = nullptr;
    for(auto e : entities) {
        vec3 pt, normal;
        if(pick(e.get(), &pt, &normal)) {
            if(event.isLeft()) {
                menu = e->getActions();
                menu->setPos(event.getPos());
                menu->setVisible(false);
                menu->onMouseDown(event);
            } else if(event.isRight()) {
                e->onMouseDown(event, mainCam);
            }
        }
        if(e->isGrabbed()) {
            grabbed = e;
            break;
        }
    }
    for(auto w : windows)
        w->onMouseDown(event);
    
    if(menuBar.getRect().contains(mousePos)) {
        menuBar.onMouseDown(event);
    }
}

void game::onMouseUp(MouseEvent event)
{
    if(menu != nullptr)
        menu->onMouseUp(event);
    menuBar.onMouseUp(event);
    
    // if anything was grabbed, send onAccept events to programs
    if(grabbed != nullptr) {
        cout << "!!!" << grabbed->getName() << endl;
        for(auto w : windows)
            w->onAccept(event, grabbed);
    } else {
        for(auto w : windows)
            w->onMouseUp(event);
    }
    
    for(auto e : entities)
        e->onMouseUp(event);
    
    grabbed = nullptr;
}

void game::onMouseMove(MouseEvent event)
{
    if(uiFocused) {
        if(menu != nullptr)
            menu->onMouseMove(event);
        for(auto w : windows)
            w->onMouseMove(event);
    } else {
    }
}

void game::onMouseDrag(MouseEvent event)
{
    if(menu != nullptr)
        menu->onMouseDrag(event);
    for(auto e : entities)
        e->onMouseDragged(event);
    for(auto w : windows)
        w->onMouseDrag(event);
}

void game::onKeydown(KeyEvent event)
{
    mainCam.onKeyDown(event);
    switch(event.getCode()) {
        case KeyEvent::KEY_ESCAPE:
            exit(0);
            break;
        default:
            break;
    }
    for(auto w : windows)
        w->onKeydown(event);
}

vec3 screenToWorld(const ivec2 &scr, float z)
{
    // Build the viewport (x, y, width, height).
    vec2 offset = gl::getViewport().first;
    vec2 size = gl::getViewport().second;
    vec4 viewport = vec4( offset.x, offset.y, size.x, size.y );
    
    // Calculate the view-projection matrix.
    mat4 transform = gl::getProjectionMatrix() * gl::getViewMatrix();
    
    // Calculate the intersection of the mouse ray with the near (z=0) and far (z=1) planes.
    vec3 near = glm::unProject( vec3( scr.x, size.y - scr.y, 0 ), mat4(), transform, viewport );
    vec3 far = glm::unProject( vec3( scr.x, size.y - scr.y, 100), mat4(), transform, viewport );
    
    // Calculate world position.
    return ci::lerp( near, far, ( z - near.z ) / ( far.z - near.z ) );
}
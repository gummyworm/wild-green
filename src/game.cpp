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
#include "desk.hpp"
#include "game.h"
#include "worldmap.hpp"
#include "cursor.hpp"
#include "npc.hpp"
#include "speech.hpp"
#include "ai_pace.hpp"
#include "ai_follow.hpp"
#include "door.hpp"
#include "TexturedCubeTile.hpp"
#include "dave.hpp"
#include "guimanager.hpp"
#include "button.hpp"
#include "ButtonGrid.hpp"
#include "editorpalette.hpp"
#include "trigger.hpp"
#include <glm/gtx/perpendicular.hpp>
#include "textedit.hpp"
#include "console.hpp"
#include "blender.hpp"
#include "fx.hpp"
#include "herd_organizer.hpp"

float game::deltaTime;

// rendering
PlayerCam mainCam;
static gl::GlslProgRef postPass;
gl::GlslProgRef game::uiProg;
static gl::FboRef fbo;
static gl::FboRef pixelateFbo;

// input
static bool uiFocused; // if true, UI events are handled
static vec2 mousePos;
static shared_ptr<Entity> grabbed;  // the entity that the client is "holding"

// UI elements
static MenuBar menuBar;
static Party party;
static Desk desk;
static ContextMenu *menu;
static shared_ptr<Conversation> convo;
static Cursor cursor;

// game state
static vector<shared_ptr<Widget>> windows;
static vector<shared_ptr<Entity>> entities;
static vector<shared_ptr<Entity>> toRemove;
static vector<shared_ptr<Trigger>> triggers;

static vector<TriMesh*> pickable;
static WorldMap world;
static Timer gameTimer;
static float gameTimePrev;

static vec3 grabbedPt;
static vec3 grabbedNormal;
static float grabbedDist;

static ColorA clearColor = ColorA(0,0,1,1);

// managers
SpeechManager game::speechMgr;
GUIManager game::guiMgr;

shared_ptr<Entity> game::getPicked(ivec2 mousePos)
{
    for(auto e : entities) {
        vec3 grabbedPt, grabbedNormal;
        if(game::pick(e.get(), nullptr, mousePos, &grabbedPt, &grabbedNormal)) {
            return e;
        }
    }
    return nullptr;
}

bool game::pick(Entity *e, WidgetWindow *win, vec2 pos, vec3 *pickedPoint, vec3 *pickedNormal)
{
    TriMesh *mesh = e->getMesh();
    mat4 transform = e->getTransform();
    
    float w, h;
    float x = pos.x;
    float y = pos.y;
    if(win == nullptr) {
        w = properties::screenWidth;
        h = properties::screenHeight;
    } else {
        w = win->getInternalRect().getWidth();
        h = win->getInternalRect().getHeight();
    }
    
    // Generate a ray from the camera into our world. Note that we have to
    // flip the vertical coordinate.
    float u = x / w;
    float v = y / h;
    Ray ray = mainCam.generateRay(u, 1.0f-v, w/h);
    
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

vec3 game::screenToWorld(ivec2 pos, float dist)
{
    float u = ((float)pos.x / properties::screenWidth);
    float v = 1.0f - ((float)pos.y / properties::screenHeight);
    
    vec3 o = mainCam.getPos() + mainCam.getViewDirection() * dist;
    vec3 normal = glm::perp(vec3(mainCam.getViewDirection()), vec3(mainCam.getViewDirection())+vec3(0,1,0));
    Ray r = mainCam.generateRay(u, v, properties::aspectRatio);
    
    float t;
    // if mouse Y is over halfway up the screen, move at maximum distance
    if(v > 0.5f && !r.calcPlaneIntersection(o, normal, &t)) {
        //return;
    } else if(!r.calcPlaneIntersection(o, normal, &t)) {
    }
    return r.calcPosition(t);
}

vector<shared_ptr<Entity>> game::entitiesInRadius(vec3 origin, float radius)
{
    vector<shared_ptr<Entity>> matched;
    for(auto e : entities) {
        if(distance(e->getPos(), origin) <= radius) {
            matched.push_back(e);
        }
    }
    return matched;
}

void test()
{
    shared_ptr<Entity> carlosE(new Entity("carlos"));
    shared_ptr<Entity> petE(new Entity("pet"));
    
    carlosE->setPos(vec3(0,0,-10));
    mainCam.setViewer(carlosE);
    carlosE->setVisible(false);
    
    shared_ptr<PartyMember> camel(new PartyMember(carlosE, "carlos", "carlos.png", "carlos_hand.png"));
    shared_ptr<PartyMember> camel2(new PartyMember(petE, "pet", "pet2001.png", "cursor.png"));
    party.addMember(camel);
    party.addMember(camel2);
    
    convo = shared_ptr<Conversation>(new Conversation());
    camel->joinConversation(convo);
    camel->say("HELLO!");
    
    shared_ptr<Entity> testE(new Entity("teapot", "", nullptr, "carlos.png"));
    testE->setPos(vec3(0,3.0f,0));
    
    shared_ptr<PartyMember> camel3(new PartyMember(testE, "carlos", "carlos.png", "carlos_hand.png"));
    party.addMember(camel3);
    
    shared_ptr<Blender> fileViewer(new Blender());
    game::guiMgr.addWidget(fileViewer);
    
    shared_ptr<HerdOrganizer> herdOrg(new HerdOrganizer(carlosE, 8,8));
    game::guiMgr.addWidget(herdOrg);
    
    shared_ptr<Console> textEdit(new Console());
    textEdit->setPos(ivec2(300, 200));
    game::guiMgr.addWidget(textEdit);
    
    shared_ptr<EditorPalette> editor(new EditorPalette());
    editor->setPos(ivec3(280, 300, 0));
    game::guiMgr.addWidget(editor);
    
    shared_ptr<CombatLauncher> combatWin(new CombatLauncher(testE));
    combatWin->setPos(ivec2(0,200));
    game::guiMgr.addWidget(combatWin);
    
    
    testE->addAction("OPEN WITH FILE VIEWER", [fileViewer]() mutable {
        fileViewer->launch();
    });
    testE->addAction("FIGHT", [combatWin]() mutable {
        combatWin->launch();
    });
    
    testE->addAction("test action3", [](){});
    testE->addAction("test action4", [](){});
    
    entities.push_back(carlosE);
    entities.push_back(testE);
    
    // setup the menu
    shared_ptr<ContextMenu> fileMenu(new ContextMenu("FILE"));
    fileMenu->addItem("test", [](){});
    fileMenu->addItem("test", [](){});
    fileMenu->addItem("test", [](){});
    menuBar.addSubmenu(fileMenu);
    shared_ptr<ContextMenu> editMenu(new ContextMenu("EDIT"));
    menuBar.addSubmenu(editMenu);

    shared_ptr<Inspector> win(new Inspector(gl::Batch::create(geom::Cube(), gl::getStockShader(gl::ShaderDef().lambert().color()))));
    windows.push_back(win);
    
    vector<vector<vector<int>>> worldTiles =
    {
        {
            {0,1,1,1,1,1,0,0,0,0,0,0,0},
            {1,1,1,1,1,1,0,0,0,0,0,0,0},
            {1,1,1,1,1,1,0,0,0,0,0,0,0},
            {1,1,1,1,1,1,0,0,0,0,0,0,0},
            {1,1,1,1,1,1,0,0,0,0,0,0,0},
            {1,1,1,1,1,1,0,0,0,0,0,0,0},
        },
        {
            {0,0,1,0,1,0,0,0,0,0,0,0,0},
            {0,0,1,0,1,0,0,0,0,0,0,3,2},
            {0,0,1,0,1,0,0,0,0,3,3,0,0},
            {1,1,1,0,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0},
            {1,1,1,0,1,1,0,0,0,0,0,0,0},
        },
        {
            {0,0,1,0,1,0,0,0,0,0,0,0,0},
            {0,0,1,0,1,0,0,0,0,0,0,0,0},
            {0,0,1,0,1,0,0,0,0,0,0,0,0},
            {1,1,1,0,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0},
            {1,1,1,0,1,1,0,0,0,0,0,0,0},
        },
        {
            {1,1,1,1,1,1,0,0,0,0,0,0,0},
            {1,1,1,1,1,1,0,0,0,0,0,0,0},
            {1,1,1,1,1,1,0,0,0,0,0,0,0},
            {1,1,1,1,1,1,0,0,0,0,0,0,0},
            {1,1,1,1,1,1,0,0,0,0,0,0,0},
            {1,1,1,1,1,1,0,0,0,0,0,0,0},
        },
    };
    
    world.load(worldTiles);
    
    triggers.push_back(shared_ptr<TextTrigger>(new TextTrigger(vec3(-3,0,-3), 2, "TRIGGERED!!!")));
    
    desk.setImage("desk.png");

    shared_ptr<NPC> testNpc2(new NPC("Frank"));
    shared_ptr<NPC> testNpc(new Dave());

    testNpc2->setAIPattern(shared_ptr<AIPattern>(new AIPace(vec3(0,0,0), vec3(10,0,0))));
    testNpc->setAIPattern(shared_ptr<AIPattern>(new AIFollow(testNpc2.get())));
    entities.push_back(testNpc);
    entities.push_back(testNpc2);
    testNpc->setPos(vec3(8, 1, 4));
    testNpc2->setPos(vec3(12, 1, 4));
    testNpc->say("Wudddup?");
    testNpc2->say("Mmmhmm?");
    
    shared_ptr<Door> door(new SlidingDoor(vec3(3,1,0), vec3(3,2,0)));
    entities.push_back(door);
}

void game::setup()
{
    uiProg = gl::getStockShader(gl::ShaderDef().lambert().color());
    postPass = gl::GlslProg::create(loadAsset("postpass.vert"), loadAsset("postpass.frag"));
    fbo = gl::Fbo::create(getWindowWidth(), getWindowHeight());
    pixelateFbo = gl::Fbo::create(properties::screenWidth, properties::screenHeight);
    
    gameTimer.start();
    
    cursor = Cursor("cursor.png");
    test();
}

void game::start()
{
    
}

void game::draw()
{
    gl::enableDepthWrite();
    gl::enableDepthRead();
    gl::enableDepth();
    gl::enableAlphaBlending();
    
    fbo->bindFramebuffer();
    gl::clear(clearColor);
    
    // draw the scene
    gl::setMatrices(mainCam);
    
    gl::color(1.0f, 1.0f, 1.0f);
    gl::drawCube(vec3(-2, 0, 0), vec3(2,2,2));
    gl::color(0.7f, 0.3f, 0.65f);
    
    for(auto e : entities) {
        if(!e->isEnabled())
            continue;
            e->draw(mainCam);
    }
    for(auto t : triggers) {
        t->draw();
    }
    
    world.draw();
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
    gl::disable(GL_TEXTURE_2D);
    gl::enable(GL_TEXTURE_2D);
    
    gl::clear(Color(1,1,1));
    gl::color(Color(1,1,1));
    gl::viewport(0, 0, getWindowWidth(), getWindowHeight());
    auto tex = pixelateFbo->getColorTexture();
    tex->setMinFilter(GL_NEAREST);
    tex->setMagFilter(GL_NEAREST);
    tex->bind(0);
    gl::draw(tex, tex->getBounds());
    
    // draw the UI
    gl::disableDepthRead();
    gl::disableDepthWrite();
    uiProg->bind();
    
    for(auto e : entities) {
        if(!e->isEnabled())
            continue;
        gl::color(1,1,1,1);
        e->label(mainCam);
    }
    
    gl::color(1,1,1);
    gl::setMatricesWindow(properties::screenSize);
    //desk.draw();
    party.draw();
    menuBar.draw();
    menuBar.apply();
    
    if(menu != nullptr) {
        menu->draw();
        menu->apply();
    }
    speechMgr.draw();
    guiMgr.draw();
    cursor.draw();

    gl::enableDepthRead();
    gl::enableDepthWrite();
}

void game::resize(ivec2 size)
{
    mainCam.onResize();
    fbo = gl::Fbo::create(size.x, size.y);
    for(auto e : entities) {
        if(!e->isEnabled())
            continue;
        e->onResize();
    }
}

void game::add(Entity *e, vec3 pos)
{
    e->setPos(pos);
    entities.push_back(shared_ptr<Entity>(e));
}

void game::remove(Entity *e)
{
    toRemove.push_back(shared_ptr<Entity>(e));
}

void game::update()
{
    deltaTime = gameTimer.getSeconds() - gameTimePrev;
    gameTimePrev += deltaTime;
    
    if(!toRemove.empty()) {
        vector<shared_ptr<Entity>> updated;
        for(auto e : toRemove) {
            if(find(toRemove.begin(), toRemove.begin(), e) == toRemove.end()) {
                updated.push_back(e);
            }
        }

        entities = updated;
        toRemove.clear();
    }
    
    for(auto e : entities) {
        if(!e->isEnabled())
            continue;
        e->update();
        for(auto t : triggers) {
            t->test(e.get());
        }
    }
    fx::update();
    guiMgr.update();
    speechMgr.update();
}

void game::onMouseDown(MouseEvent event)
{
    mousePos = event.getPos();
    menu = nullptr;
    for(auto e : entities) {
        if(pick(e.get(), nullptr, mousePos, &grabbedPt, &grabbedNormal)) {
            if(event.isRight()) {
                menu = e->getActions().get();
                menu->setPos(event.getPos());
                menu->setVisible(false);
                menu->onMouseDown(event);
            } else if(event.isLeft()) {
                if(e->getFlags().grabbable)
                    grabbed = e;
                e->onMouseDown(event, mainCam);
            }
        }
    }
    guiMgr.onMouseDown(event);
    speechMgr.onMouseDown(event);
    
    if(menuBar.getRect().contains(mousePos)) {
        menuBar.onMouseDown(event);
    }
    
    party.onMouseDown(event);
}

void game::onMouseUp(MouseEvent event)
{
    if(menu != nullptr)
        menu->onMouseUp(event);
    menuBar.onMouseUp(event);
    
    // if anything was grabbed, send onAccept events to programs
    if(grabbed != nullptr) {
        if(!guiMgr.onAccept(event, grabbed)) {
            shared_ptr<Entity> e = getPicked(event.getPos());
            if(e != nullptr)
                e->onAccept(grabbed);
        }
    } else {
        guiMgr.onMouseUp(event);
    }
    
    for(auto e : entities)
        e->onMouseUp(event);
    
    party.onMouseUp(event);
    
    speechMgr.onMouseUp(event);
    
    grabbed = nullptr;
}

void game::onMouseMove(MouseEvent event)
{
    cursor.onMouseMove(event);

    if(uiFocused) {
        if(menu != nullptr)
            menu->onMouseMove(event);
        guiMgr.onMouseMove(event);
    } else {
    }
}

void game::onMouseDrag(MouseEvent event)
{
    cursor.onMouseMove(event);
    
    if(menu != nullptr)
        menu->onMouseDrag(event);
    menuBar.onMouseDrag(event);
    
    if(grabbed != nullptr) {
        float u = ((float)event.getX()) / properties::screenWidth;
        float v = 1.0f - ((float)event.getY() / properties::screenHeight);
    
        Ray r = mainCam.generateRay(u, v, properties::aspectRatio);
        gl::color(Colorf(0.5, 0.5, 0));
        
        float t;
        vec3 normal = glm::perp(vec3(mainCam.getViewDirection()), vec3(mainCam.getViewDirection())+vec3(0,1,0));
        
        //if(!r.calcPlaneIntersection(grabbedPt, grabbedNormal, &t)) {
        
        // if mouse Y is over halfway up the screen, move at maximum distance
        if(v > 0.5f && !r.calcPlaneIntersection(grabbedPt, normal, &t)) {
            //return;
        } else if(!r.calcPlaneIntersection(grabbedPt, normal, &t)) {
            return;
        }
        
        vec3 p = r.calcPosition(t);
        //p.z = grabbedPt.z; //grabbedPt.z - mainCam.getPos().z;
        
        //grabbed->setPos(p);
        //grabbedDist = grabbedPt.z - mainCam.getPos().z; //glm::distance(grabbedPt, mainCam.getPos());
        //grabbed->setPos(r.getOrigin() + (r.getDirection() * grabbedDist));
    }

    for(auto e : entities)
        e->onMouseDragged(event);
    
    guiMgr.onMouseDrag(event);
    speechMgr.onMouseDrag(event);
    party.onMouseDrag(event);
}

shared_ptr<Entity> game::colliding(Entity *entity)
{
    
    for(auto e : entities) {
        if(!e->isVisible())
            continue;
        if(e->getAABB().intersects(entity->getAABB())) {
            return e;
        }
    }
    return nullptr;
}

void game::onKeydown(KeyEvent event)
{
    vec3 old = mainCam.getPos();
    
    if(guiMgr.onKeydown(event))
        return;
    
    mainCam.onKeyDown(event);
    if(world.containsPoint(mainCam.getPos() + vec3(0,1,0))) {
        mainCam.setPos(old);
    }
    
    vec3 delta = mainCam.getPos() - old;
    for(auto p : party.members) {
        p->onMove(mainCam.getDir(), delta);
    }
    
    switch(event.getCode()) {
        case KeyEvent::KEY_ESCAPE:
            exit(0);
            break;
        case properties::KEY_NEXT_PARTY_MEMBER:
            party.setPrimaryNext();
            break;
        default:
            break;
    }
}

void game::say(Entity *speaker, string msg)
{
    speechMgr.say(speaker, msg);
}

void game::grab(Entity *e)
{
    grabbed = shared_ptr<Entity>(e);
}

void game::setClearColor(ColorA color)
{
    clearColor = color;
}


/*
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
*/


// Unproject a coordinate back to to camera
vec3 unproject(const vec3 & point)
{
    
    // Find the inverse Modelview-Projection-Matrix
    mat4 mInvMVP = glm::inverse(gl::getProjectionMatrix() * gl::getModelMatrix());

    // Transform to normalized coordinates in the range [-1, 1]
    vec4 pointNormal;
    pointNormal.x = (point.x - 0) / 640.0f * 2.0f - 1.0f;
    pointNormal.y = (point.y - 0) / 480.0f * 2.0f;
    pointNormal.z = 2.0f * point.z - 1.0f;
    pointNormal.w = 1.0f;
    
    // Find the object's coordinates
    vec4 pointCoord = mInvMVP * pointNormal;
    if (pointCoord.w != 0.0f)
        pointCoord.w = 1.0f / pointCoord.w;
    
    // Return coordinate
    return vec3(
                 pointCoord.x * pointCoord.w,
                 pointCoord.y * pointCoord.w,
                 pointCoord.z * pointCoord.w);
    
}

// Convert mouse position to 3D
vec3 screenToWorld(const ivec2& point, float z)
{
    // Find near and far plane intersections
    vec3 point3f = vec3((float)point.x, getWindowHeight() * 0.5f - (float)point.y, 0.0f);
    vec3 nearPlane = unproject(point3f);
    vec3 farPlane = unproject(vec3(point3f.x, point3f.y, 1.0f));
    
    // Calculate X, Y and return point
    float theta = (0.0f - nearPlane.z) / (farPlane.z - nearPlane.z);
    return vec3(
                 nearPlane.x + theta * (farPlane.x - nearPlane.x),
                 nearPlane.y + theta * (farPlane.y - nearPlane.y),
                 0.0f
                 );
}


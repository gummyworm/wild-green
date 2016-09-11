#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/gl/Fbo.h"
#include "cinder/app/RendererGl.h"
#include "cinder/TriMesh.h"
#include "conversation.hpp"
#include "entity.hpp"
#include "materials.h"
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

using namespace ci;
using namespace ci::app;
using namespace std;

class digitalApp : public App {
     Materials materials;
     
     CameraOrtho guiCam;
     vec2 mousePos;
     
     // UI elements
     MenuBar menuBar;
     Party party;
     ContextMenu *menu;
     vector<shared_ptr<Widget>> windows;
     vector<TriMesh*> pickable;
     vector<shared_ptr<Entity>> entities;
     Conversation convo;
     
     shared_ptr<Entity> grabbed;
     
     // rendering
     gl::GlslProgRef postPass;
     gl::GlslProgRef uiProg;
     gl::FboRef fbo;
     gl::FboRef pixelateFbo;
     
     bool uiFocused;
     
     ivec2 mouseToScreen(ivec2 mouse) {return vec2(mouse) * vec2(properties::screenSize) / vec2(getWindowSize());}
public:
     digitalApp();
     void createPostpass();
     void setup() override;
     void mouseDown(MouseEvent event) override;
     void mouseMove(MouseEvent event) override;
     void mouseUp(MouseEvent event) override;
     void mouseDrag(MouseEvent event) override;
     void keyDown(KeyEvent event) override;
     
     void resize() override;
     void update() override;
     void draw() override;
     bool pick(Entity *e, vec3 *pickedPoint, vec3 *pickedNormal);
};

digitalApp::digitalApp()
:App(),
menu(nullptr),
convo()
{
}

void digitalApp::createPostpass() {
     uiProg = gl::getStockShader(gl::ShaderDef().lambert().color());
     postPass = gl::GlslProg::create(loadAsset("postpass.vert"), loadAsset("postpass.frag"));
     fbo = gl::Fbo::create(getWindowWidth(), getWindowHeight());
     pixelateFbo = gl::Fbo::create(properties::screenWidth, properties::screenHeight);
}

void digitalApp::setup()
{
     //getWindow()->setBorderless();
     setWindowSize(640, 480);
     
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
     
     createPostpass();
     
     shared_ptr<Inspector> win(new Inspector(gl::Batch::create(geom::Cube(), gl::getStockShader(gl::ShaderDef().lambert().color()))));
     windows.push_back(win);
     
}

void digitalApp::mouseDown(MouseEvent event)
{
     mousePos = mouseToScreen(event.getPos());
     event.setPos(mousePos);
     
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

void digitalApp::mouseMove(MouseEvent event)
{
     mousePos = mouseToScreen(event.getPos());
     event.setPos(mousePos);
     
     if(uiFocused) {
          if(menu != nullptr)
               menu->onMouseMove(event);
          for(auto w : windows)
               w->onMouseMove(event);
     } else {
     }
}

void digitalApp::mouseUp(MouseEvent event)
{
     mousePos = mouseToScreen(event.getPos());
     event.setPos(mousePos);
     
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

void digitalApp::mouseDrag(MouseEvent event)
{
     mousePos = mouseToScreen(event.getPos());
     event.setPos(mousePos);
     
     if(menu != nullptr)
          menu->onMouseDrag(event);
     for(auto e : entities)
          e->onMouseDragged(event);
     for(auto w : windows)
          w->onMouseDrag(event);
}

void digitalApp::keyDown(KeyEvent event)
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

void digitalApp::update()
{
     for(auto e : entities)
          e->update();
}

void digitalApp::draw()
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

void digitalApp::resize()
{
     mainCam.onResize();
     fbo = gl::Fbo::create(getWindowWidth(), getWindowHeight());
     
     for(auto e : entities) {
          e->onResize();
     }
}

bool digitalApp::pick(Entity *e, vec3 *pickedPoint, vec3 *pickedNormal)
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

CINDER_APP(digitalApp, RendererGl)
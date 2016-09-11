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
     ivec2 mouseToScreen(ivec2 mouse) {return vec2(mouse) * vec2(properties::screenSize) / vec2(getWindowSize());}
public:
     digitalApp();
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
:App()
{
}

void digitalApp::setup()
{
     //getWindow()->setBorderless();
     setWindowSize(640, 480);
     game::setup();
     
}

void digitalApp::mouseDown(MouseEvent event)
{
     event.setPos(mouseToScreen(event.getPos()));
     game::onMouseDown(event);
}

void digitalApp::mouseMove(MouseEvent event)
{
     event.setPos(mouseToScreen(event.getPos()));
     game::onMouseMove(event);
     
}

void digitalApp::mouseUp(MouseEvent event)
{
     event.setPos(mouseToScreen(event.getPos()));
     game::onMouseUp(event);
}

void digitalApp::mouseDrag(MouseEvent event)
{
     event.setPos(mouseToScreen(event.getPos()));
     game::onMouseDrag(event);
}

void digitalApp::keyDown(KeyEvent event)
{
     game::onKeydown(event);
}

void digitalApp::update()
{
     game::update();
}

void digitalApp::draw()
{
     game::draw();
}

void digitalApp::resize()
{
     game::resize(getWindowSize());
}

CINDER_APP(digitalApp, RendererGl)
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class digitalApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void digitalApp::setup()
{
}

void digitalApp::mouseDown( MouseEvent event )
{
}

void digitalApp::update()
{
}

void digitalApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( digitalApp, RendererGl )

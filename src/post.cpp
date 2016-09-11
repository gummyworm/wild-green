//
//  post.cpp
//  pool
//
//  Created by Bryce on 8/20/16.
//
//

#include "materials.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void Materials::setup()
{
	postPass = gl::GlslProg::create(loadAsset("postpass.vert"), loadAsset("postpass.frag"));
}
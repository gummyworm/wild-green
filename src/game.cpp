//
//  game.cpp
//  digital
//
//  Created by Bryce on 9/6/16.
//
//

#include "game.h"


PlayerCam mainCam;

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
#version 410 core
layout(location = 0) in vec3 vp;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 transform;
void main(){
    gl_Position = projection*view*vec4(vp.x, vp.y, vp.z, 1.0);
}
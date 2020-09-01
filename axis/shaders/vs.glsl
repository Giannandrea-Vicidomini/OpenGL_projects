#version 330 core

layout (location = 0)in vec3 pos;

uniform mat4 pTransform;
uniform mat4 proj;

out vec4 varyingColor;

void main(){
    gl_Position = proj*pTransform*vec4(pos,1);
    varyingColor = vec4(pos,1)*0.5+vec4(0.5,0.5,0.5,0.5);
}
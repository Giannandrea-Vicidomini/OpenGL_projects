#version 330 core

layout (location = 0)in vec4 pos;
layout (location = 1)in vec2 tCoord;
out vec4 colors;
out vec2 uv;

void main(){
    gl_Position = pos;
    colors = pos;
    uv = tCoord;
}
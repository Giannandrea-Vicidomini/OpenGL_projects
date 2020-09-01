#version 330 core

layout(location = 1)in vec4 pos;
uniform mat4 projectMat;
uniform mat4 viewMat;

void main(){
    gl_Position = pos;
}
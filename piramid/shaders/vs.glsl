#version 330

layout (location = 0)in vec4 pos;
out vec4 colors;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;

void main(){
    gl_Position = projMatrix*viewMatrix*pos;
    colors = pos * 0.5 + vec4(0.5,0.5,0.5,1);
}
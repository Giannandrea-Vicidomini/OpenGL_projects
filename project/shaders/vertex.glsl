#version 330 core

layout (location = 0) in vec3 pos;
smooth out vec4 vColors;
uniform mat4 proj;
uniform mat4 viewMatrix;

void main(){
    
    vColors = vec4(pos,1)*0.5+vec4(0.5,0.5,0.5,0.5);
    gl_Position = proj*viewMatrix*vec4(pos,1);
}
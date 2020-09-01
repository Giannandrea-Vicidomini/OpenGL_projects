#version 330 core

out vec4 colors;
smooth in vec4 vColors;

float ratio = gl_FragCoord.y /600;

void main(){
    
    colors = mix(vec4(1,0,0,1),vec4(0,1,1,1),ratio);

}
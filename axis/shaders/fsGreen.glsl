#version 330 core

out vec4 color;
in vec4 varyingColor;

void main(){
    color = varyingColor;

}
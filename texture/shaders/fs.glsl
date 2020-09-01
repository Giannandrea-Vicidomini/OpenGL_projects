#version 330 core

in vec4 colors;
in vec2 uv;
out vec4 fragCol;
uniform sampler2D texel;


void main(){
    //fragCol=colors * 0.5 + vec4(0.5,0.5,0.5,0.5);
    //fragCol = vec4(uv,0,1);
    fragCol = texture(texel,uv);
}
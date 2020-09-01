#version 330 core

layout(location = 0)in vec3 pos;
layout(location = 1)in vec3 norm;
layout(location = 2)in vec2 texCoord;

out vec4 fColor;
out vec2 uv;
uniform mat4 modMat;
uniform mat4 projMat;
uniform mat4 viewMat;
uniform vec3 lightPosition;

out vec3 toLightVector;
out vec3 worldNormal;

void main(){
    //lightPosition = vec3(0,0,-10);
    vec4 worldPosition = modMat*vec4(pos,1);

    worldNormal = (modMat*vec4(norm,0.0)).xyz;
    toLightVector = (lightPosition) - (worldPosition).xyz;
    
    gl_Position = projMat*viewMat*worldPosition;

    fColor = vec4(pos,1)*0.5+vec4(0.5,0.5,0.5,0.5);;
    uv = texCoord;
}
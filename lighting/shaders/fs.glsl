#version 330 core

in vec4 fColor;
in vec2 uv;
out vec4 color;
in vec3 toLightVector;
in vec3 worldNormal;

void main(){
    
    vec3 unitNormal = normalize(worldNormal);
    vec3 unitLight = normalize(toLightVector);
    
    float brightness = max(dot(unitLight,unitNormal),0);
    vec3 diffuse = brightness * vec3(1,1,1);
    //color = brightness*fColor;
    color = (vec4(diffuse,1)*fColor)+(fColor*vec4(0.09f,0.09f,0.092f,1));
    //color = vec4(uv,0,1);
    //color = fColor*3;
    //color = vec4(0,1,1,1)+vec4(0,6,6,1);
}
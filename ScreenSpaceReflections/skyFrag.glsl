#version 460 core

out vec4 color;

in vec3 texCoord;

uniform samplerCube cubeTex;

void main()
{    
    color = texture(cubeTex, texCoord);
}
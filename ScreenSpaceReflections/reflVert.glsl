#version 460 core

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

layout(location=0) in vec3 inVertex;
layout(location=1) in vec3 inNormal;

uniform vec3 inEyePos;

out vec3 eyePos;
out vec4 fragWorldPos;
out vec3 fragWorldNor;
out vec3 texCoord;

void main(void)
{
// 
	fragWorldPos = modelMat * vec4(inVertex, 1);
	fragWorldNor = normalize(inverse(transpose(mat3x3(modelMat))) * inNormal);
	texCoord = normalize(reflect(normalize(fragWorldPos.xyz - inEyePos), fragWorldNor));
	eyePos = inEyePos;
    gl_Position = projMat * viewMat * modelMat * vec4(inVertex, 1);
}


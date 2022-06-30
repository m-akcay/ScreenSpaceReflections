#version 460 core

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

layout(location=0) in vec3 inVertex;
layout(location=1) in vec3 inNormal;
layout(location=2) in vec2 inTexCoord;

out vec4 fragWorldPos;
out vec3 fragWorldNor;
out vec2 texCoord;

void main(void)
{
	// Compute the world coordinates of the vertex and its normal.
	// These coordinates will be interpolated during the rasterization
	// stage and the fragment shader will receive the interpolated
	// coordinates.

	fragWorldPos = modelMat * vec4(inVertex, 1);
	fragWorldNor = inverse(transpose(mat3x3(modelMat))) * inNormal;
	texCoord = inTexCoord;

    gl_Position = projMat * viewMat * modelMat * vec4(inVertex, 1);
}


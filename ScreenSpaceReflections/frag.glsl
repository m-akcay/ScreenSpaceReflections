#version 460 core

// All of the following variables could be defined in the OpenGL
// program and passed to this shader as uniform variables. This
// would be necessary if their values could change during runtim.
// However, we will not change them and therefore we define them 
// here for simplicity.

vec3 I = vec3(1, 1, 1);          // point light intensity
vec3 Iamb = vec3(0.8, 0.8, 0.8); // ambient light intensity
vec3 kd = vec3(1, 0.2, 0.2);     // diffuse reflectance coefficient
vec3 ka = vec3(0.3, 0.3, 0.3);   // ambient reflectance coefficient
vec3 ks = vec3(0.8, 0.8, 0.8);   // specular reflectance coefficient
vec3 lightPos = vec3(-5, 5, -5);   // light position in world coordinates

uniform vec3 eyePos;
uniform float reflectiveness = 0;

in vec4 fragWorldPos;
in vec3 fragWorldNor;
in vec2 texCoord;


layout (location=0) out vec4 fragColor;
layout (location=1) out vec4 outNormal;
layout (location=2) out vec4 reflectionVal;
layout (location=3) out vec4 posWS;

void main(void)
{
	// Compute lighting. We assume lightPos and eyePos are in world
	// coordinates. fragWorldPos and fragWorldNor are the interpolated
	// coordinates by the rasterizer.
//	vec3 kd = texture(tex, texCoord).rgb;
	vec3 L = normalize(lightPos - vec3(fragWorldPos));
	vec3 V = normalize(eyePos - vec3(fragWorldPos));
	vec3 H = normalize(L + V);
	vec3 N = normalize(fragWorldNor);

	float NdotL = dot(N, L); // for diffuse component
	float NdotH = dot(N, H); // for specular component

	vec3 diffuseColor = I * kd * max(0, NdotL);
	vec3 specularColor = I * ks * pow(max(0, NdotH), 100);
	vec3 ambientColor = Iamb * ka;

	fragColor = vec4(diffuseColor + specularColor + ambientColor, 1);
	outNormal = vec4(N, 1);
	reflectionVal = vec4(reflectiveness, 0.0f, 0.0f, 1.0f);
	posWS = fragWorldPos;
}

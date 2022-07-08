#version 460 core

in vec2 texCoord;

uniform sampler2D colorTexture;
uniform sampler2D depthTexture;
uniform sampler2D normalTexture;

out vec4 fragColor;

float near = 0.1; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
//     for depth
    float rawZ = texture(depthTexture, texCoord).r;
	float depth = LinearizeDepth(rawZ) / far; // divide by far for demonstration

//    fragColor = vec4(0.0f);

    fragColor = texture(colorTexture, texCoord);
//    fragColor = vec4(vec3(depth), 1.0);
//    fragColor = texture(normalTexture, texCoord);

}
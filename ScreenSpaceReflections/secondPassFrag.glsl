#version 460 core

in vec2 texCoord;

uniform sampler2D colorTexture;
uniform sampler2D depthTexture;
uniform sampler2D normalTexture;
uniform sampler2D reflectionTexture;
uniform sampler2D posTexture;
uniform samplerCube cubeTexture;

uniform mat4 gProjectionMatrix;
uniform vec3 eyePos;

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

    float reflective = texture(reflectionTexture, texCoord).r;

    if (reflective > 0.1)
    {
        vec3 posWS = texture(posTexture, texCoord).xyz;
        vec3 normal = texture(normalTexture, texCoord).xyz;
        vec3 reflected = reflect(normalize(posWS - eyePos), normal);
        bool hit = false;
        vec2 hitPos = vec2(0.0f);

        float fstep = 0.01f;
        float thisDepth = texture(depthTexture, texCoord).r;

        for (int i = 0; i < 16; i++)
        {
            vec2 vstep = vec2(fstep * i, fstep * i);
            vec2 nextPos = texCoord + vstep * reflected.xy;
            
            float nextDepth = texture(depthTexture, nextPos).r;
            if (thisDepth > nextDepth + 0.0001f)
            {
                hit = true;
                hitPos = nextPos;
                break;
            }
        }

        if (hit)
        {
            fragColor = texture(colorTexture, hitPos);
//            fragColor = vec4(0, 0, 0, 1);
        }
        else
        {
            fragColor = texture(cubeTexture, reflected);
        }
    }
    else
    {
        fragColor = texture(colorTexture, texCoord);
    }


}

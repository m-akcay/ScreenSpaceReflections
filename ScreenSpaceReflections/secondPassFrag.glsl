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
vec4 rayMarch(inout vec3 position, vec3 direction, out float delta);

void main()
{
//     for depth
//    float rawZ = texture(depthTexture, texCoord).r;
//	float depth = LinearizeDepth(rawZ) / far; // divide by far for demonstration

//    fragColor = vec4(0.0f);

    float reflective = texture(reflectionTexture, texCoord).r;

    if (reflective > 0.1)
    {
        vec3 posWS = texture(posTexture, texCoord).xyz;
        vec3 normal = texture(normalTexture, texCoord).xyz;
        vec3 reflected = reflect(normalize(posWS - eyePos), normal);
        bool hit = false;
        vec2 hitPos = vec2(0.0f);

        float fstep = 0.001f;
        float thisDepth = texture(depthTexture, texCoord).r;

        for (int i = 0; i < 16; i++)
        {
            vec2 vstep = vec2(fstep * i, fstep * i);
            vec2 nextPos = texCoord + vstep * reflected.xy;
            
            float nextDepth = texture(depthTexture, nextPos).r;
            if (thisDepth > nextDepth)
            {
                hit = true;
                hitPos = nextPos;
                break;
            }
        }

        if (hit)
            fragColor = texture(colorTexture, hitPos);
        else
            fragColor = texture(cubeTexture, reflected);

    }
    else
    {
        fragColor = texture(colorTexture, texCoord);
    }


}

vec4 rayMarch(inout vec3 position, vec3 direction, out float delta)
{
 
    vec4 projectedCoords;
    float depth;
    int NUM_STEPS = 16;
    for (int STEP = 0; STEP < NUM_STEPS; ++STEP)
    {
        direction *= STEP;

        position += direction;
 
        vec4 projectedCoords = gProjectionMatrix * vec4(position, 1.0f);
        projectedCoords.xy /= projectedCoords.w;
        projectedCoords.xy = projectedCoords.xy * 0.5f + 0.5f;
 
        depth = texture(posTexture, projectedCoords.xy).z;
 
        delta = position.z - depth;
 
        // if (depth - (position.z - direction.z) < 1.2f)
        // Is the difference between the starting and sampled depths smaller than the width of the unit cube?
        // We don't want to sample too far from the starting position.
        if (direction.z - delta < 1.2f)
        {
            return vec4(projectedCoords.xy, depth, 0.0f);

            // We're at least past the point where the ray intersects a surface.
            // Now, determine the values at the precise location of intersection.
//            if (delta < 0.0f)
//            {
//                return vec4(binarySearch(position, direction, delta), 1.0f);
//            }
        }
    }
    return vec4(projectedCoords.xy, depth, 0.0f);
}
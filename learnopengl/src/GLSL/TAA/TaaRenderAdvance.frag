#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D lastFrame;
uniform sampler2D velocityBuffer;
uniform sampler2D currentFrame;
uniform float alpha;
uniform mat3 RGBToYCoCgMat;
uniform mat3 YCoCgToRGBMat;



vec2 SampleVelocityBufferWithDepthCore(vec2 uv)
{
    vec2 velocity = vec2(0);
    float minDepth = 1000000;
    for(int x=-1;x<2;x++)
    {
        for(int y=-1;y<2;y++)
        {
            vec3 currentVelocity = texture(velocityBuffer, vec2(uv.x+x, uv.y+y)).xyz;
            if(currentVelocity.z < minDepth)
            {
                minDepth = currentVelocity.z;
                velocity = currentVelocity.xy;
            }
        }
    }
    return velocity;
}

vec3 RGBToYCoCgTransFunc(vec3 rgbVec)
{
    return RGBToYCoCgMat * rgbVec;
}

vec3 YCoCgToRGBTransFunc(vec3 ycocgVec)
{
    return YCoCgToRGBMat * ycocgVec;
}


void main()
{
    vec3 col = vec3(0);
    vec2 screenCoord = vec2((TexCoords.x + 1) / 2, (TexCoords.y + 1) / 2);
    vec3 lighting = texture(currentFrame, screenCoord).xyz;
    
    vec2 velocity = SampleVelocityBufferWithDepthCore(screenCoord);
    vec3 lastColor = texture(lastFrame, screenCoord - velocity).xyz;
    vec3 finalColor = alpha * lighting + (1-alpha) * lastColor;

    FragColor = vec4(finalColor, 1);

} 
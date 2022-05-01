#version 330 core

out vec4 FragColor;
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    vec4 ClipPos;
    vec2 ScreenCoord;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
#ifndef LIGHT
uniform sampler2D lastFrame;
uniform sampler2D velocityBuffer;
uniform float alpha;
uniform mat3 RGBToYCoCgMat;
uniform mat3 YCoCgToRGBMat;
#endif
uniform vec3 lightPos;
uniform vec3 viewPos;


float ShadowCalculation(vec3 normal, vec3 lightDir, vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0f)
        return 0.0f;
    float shadow = 0.0f;
    vec2 texSize = 1.0/textureSize(shadowMap, 0);
    float bias = max(0.05 * (1 - dot(normal, lightDir)), 0.005);
    float currentDepth = projCoords.z;
    for(int x=-1;x<=1;x++)
    {
        for(int y=-1;y<=1;y++)
        {
            float fcpDepth = texture(shadowMap, projCoords.xy + vec2(x,y)*texSize).r;
            shadow += currentDepth - bias > fcpDepth ? 1.0 : 0.0;
        }
    }  
    shadow /= 9.0f;
    return shadow;
}
#ifndef LIGHT
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

#endif


void main()
{           
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(1.0);
    // Ambient
    vec3 ambient = 0.2 * color;
    // Diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // ¼ÆËãÒõÓ°
    float shadow = ShadowCalculation(normal, lightDir, fs_in.FragPosLightSpace);       
    vec3 lighting = (ambient + (1-shadow) * (diffuse + specular)) * color;    
#ifndef LIGHT
    vec2 screenCoord = vec2(fs_in.ClipPos.xy / fs_in.ClipPos.w);
    screenCoord = (screenCoord.xy + 1)/2;
    
    vec2 velocity = SampleVelocityBufferWithDepthCore(screenCoord);
    vec3 lastColor = texture(lastFrame, screenCoord - velocity).xyz;
    vec3 finalColor = alpha * lighting + (1-alpha) * lastColor;

    FragColor = vec4(finalColor, 1);
#else 
    FragColor = vec4(lighting, 1);
#endif
}
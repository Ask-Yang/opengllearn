#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    vec4 ClipPos;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform sampler2D lastFrame;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float alpha;

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
    // º∆À„“ı”∞
    float shadow = ShadowCalculation(normal, lightDir, fs_in.FragPosLightSpace);       
    vec3 lighting = (ambient + (1-shadow) * (diffuse + specular)) * color;    
    vec2 screenCoord = vec2(fs_in.ClipPos.xy / fs_in.ClipPos.w);
    screenCoord = (screenCoord.xy + 1)/2;
    
    vec3 lastColor = texture(lastFrame, screenCoord).xyz;
    vec3 finalColor = alpha * lighting + (1-alpha) * lastColor;
    FragColor = vec4(finalColor,1 );
}
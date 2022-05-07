#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D lastFrame;
uniform sampler2D velocityBuffer;
uniform sampler2D currentFrame;
uniform float alpha;
uniform mat3 RGBToYCoCgMat;
uniform mat3 YCoCgToRGBMat;

vec3 neighbors[9];

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

void SampleNeibors5(vec2 screenCoord)
{
    neighbors[0] = RGBToYCoCgTransFunc(texture(currentFrame, vec2(screenCoord.x-1, screenCoord.y-1)).xyz);
    neighbors[1] = RGBToYCoCgTransFunc(texture(currentFrame, vec2(screenCoord.x-1, screenCoord.y)).xyz);
    neighbors[2] = RGBToYCoCgTransFunc(texture(currentFrame, vec2(screenCoord.x-1, screenCoord.y+1)).xyz);
    neighbors[3] = RGBToYCoCgTransFunc(texture(currentFrame, vec2(screenCoord.x, screenCoord.y-1)).xyz);
    neighbors[4] = RGBToYCoCgTransFunc(texture(currentFrame, vec2(screenCoord.x, screenCoord.y)).xyz);
    neighbors[5] = RGBToYCoCgTransFunc(texture(currentFrame, vec2(screenCoord.x, screenCoord.y+1)).xyz);
    neighbors[6] = RGBToYCoCgTransFunc(texture(currentFrame, vec2(screenCoord.x+1, screenCoord.y-1)).xyz);
    neighbors[7] = RGBToYCoCgTransFunc(texture(currentFrame, vec2(screenCoord.x+1, screenCoord.y)).xyz);
    neighbors[8] = RGBToYCoCgTransFunc(texture(currentFrame, vec2(screenCoord.x+1, screenCoord.y+1)).xyz);
}

void CaluNeiborMinMax(inout vec3 neighborMin, inout vec3 neighborMax)
{
    neighborMin = min(neighbors[0], neighbors[1]);
    neighborMin = min(neighborMin, neighbors[2]);
    neighborMin = min(neighborMin, neighbors[3]);
    neighborMin = min(neighborMin, neighbors[4]);
    neighborMin = min(neighborMin, neighbors[5]);
    neighborMin = min(neighborMin, neighbors[6]);
    neighborMin = min(neighborMin, neighbors[7]);
    neighborMin = min(neighborMin, neighbors[8]);


    neighborMax = max(neighbors[0], neighbors[1]);
    neighborMax = max(neighborMax, neighbors[2]);
    neighborMax = max(neighborMax, neighbors[3]);
    neighborMax = max(neighborMax, neighbors[4]);
    neighborMax = max(neighborMax, neighbors[5]);
    neighborMax = max(neighborMax, neighbors[6]);
    neighborMax = max(neighborMax, neighbors[7]);
    neighborMax = max(neighborMax, neighbors[8]);
}

vec3 YCoCgAABBClip(vec4 previousColor, vec4 currentColor, vec3 colorMin, vec3 colorMax)
{
	vec3 p_clip = 0.5 * (colorMax + colorMin);
	vec3 e_clip = 0.5 * (colorMax - colorMin);
	vec4 v_clip = previousColor - vec4(p_clip, 1);
	vec3 v_unit = v_clip.rgb / e_clip;
	vec3 a_unit = abs(v_unit);
	float ma_unit = max(a_unit.x, max(a_unit.y, a_unit.z));
	if (ma_unit > 1.0)
	{
		return (vec4(p_clip, currentColor.a) + v_clip / ma_unit).rgb;
	}
	else
	{
		return previousColor.rgb;// point inside aabb
	}
}

void main()
{
    vec3 col = vec3(0);
    vec2 screenCoord = vec2((TexCoords.x + 1) / 2, (TexCoords.y + 1) / 2);
    SampleNeibors5(screenCoord);
    vec3 neighborMin = vec3(0,0,0), neighborMax = vec3(0,0,0);
    CaluNeiborMinMax(neighborMin, neighborMax);

    vec3 currentColor = neighbors[4];
    vec2 velocity = SampleVelocityBufferWithDepthCore(screenCoord);
    vec3 lastColor = texture(lastFrame, screenCoord - velocity).xyz;
    lastColor = RGBToYCoCgTransFunc(lastColor);
    //vec3  aLastColor = clamp(lastColor, neighborMin, neighborMax);
    vec3 aLastColor = YCoCgAABBClip(vec4(lastColor, 1), vec4(currentColor, 1), neighborMin, neighborMax);
    float vDis =  sqrt(velocity.x*velocity.x+velocity.y*velocity.y) + 0.2;
    lastColor = vDis * aLastColor + (1-vDis)*lastColor;
    vec3 finalColor = alpha * YCoCgToRGBTransFunc(currentColor) + (1-alpha) * YCoCgToRGBTransFunc(lastColor);

    FragColor = vec4(finalColor, 1);

} 
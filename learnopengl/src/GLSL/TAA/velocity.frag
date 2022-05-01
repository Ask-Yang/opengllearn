#version 330 core
out vec4 FragColor;

in VS_OUT{
	vec4 ClipPos;
	vec4 LastClipPos;
} fs_in;

void main()
{
	vec2 UV = vec2((fs_in.ClipPos.xy / fs_in.ClipPos.w + 1) / 2);
	vec2 lastUV = vec2((fs_in.LastClipPos.xy / fs_in.LastClipPos.w + 1) / 2);
	FragColor = vec4(UV-lastUV, gl_FragCoord.z, 1);
}
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out VS_OUT{
	vec4 ClipPos;
	vec4 LastClipPos;
} vs_out;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lastProjection;
uniform mat4 lastView;
uniform mat4 lastModel;

void main()
{
	vs_out.ClipPos = projection * view * model * vec4(position, 1);
	gl_Position = vs_out.ClipPos;
	vs_out.LastClipPos = lastProjection * lastView * lastModel * vec4(position, 1);
}

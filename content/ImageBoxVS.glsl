#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 Transform;
uniform mat4 Projection;

void main()
{
	//gl_Position =  vec4((Projection * Transform * vec4(aPos, 1.0)).xy,0,1);
	gl_Position =  vec4((Projection * Transform * vec4(aPos, 1.0)));
	//gl_Position =  Projection * vec4(aPos, 1.0);
	//gl_Position = vec4(aPos, 1.0);
	TexCoord = aTexCoord;
	//TexCoord = gl_Position.xy;


}

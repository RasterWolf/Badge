#version 330 core
//in int gl_VertexID;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 ObjectMat;

void main()
{
	gl_Position =  vec4((ObjectMat * vec4(aPos, 1.0)));
	//gl_Position = vec4(aPos, 1.0);
	TexCoord = aTexCoord;
}

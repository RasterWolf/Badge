#version 330 core
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec3 ColorMod;

void main()
{
	//vec2 uv = TexCoord;
	//uv.y = 1.0 - uv.y;
    gl_FragColor = vec4(texture2D(ourTexture, TexCoord.xy).xyzw)*vec4(ColorMod.xyz,1.0);
    //gl_FragColor = vec4(TexCoord.xy,0.0,1.0);
}

#version 130
uniform float iTime;
uniform ivec2 iResolution;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;
uniform sampler2D iChannel3;

in vec2 TexCoord;
out vec4 fragColor;

//ShaderToy

void main()
{
	// distance from center of image, used to adjust blur
	vec2 uv = TexCoord;//fragCoord.xy / iResolution.xy;
	float d = length(uv - vec2(0.5,0.5));
	
	// blur amount
	float blur = 0.0;	
	blur = (1.0 + sin(iTime*6.0)) * 0.5;
	blur *= 1.0 + sin(iTime*16.0) * 0.5;
	blur = pow(blur, 3.0);
	blur *= 0.05;
	// reduce blur towards center
	blur *= d;
	
	// final color
    vec3 col;
    col.r = texture( iChannel0, vec2(uv.x+blur,uv.y) ).r;
    col.g = texture( iChannel0, uv ).g;
    col.b = texture( iChannel0, vec2(uv.x-blur,uv.y) ).b;
	
	// scanline
	float scanline = sin(uv.y*800.0)*0.04;
	col -= scanline;
	
	// vignette
	col *= 1.0 - d * 0.5;
	
    fragColor = vec4(col,1.0);
}
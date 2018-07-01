#version 100
precision mediump float;

uniform float iTime;
uniform ivec2 iResolution;
uniform sampler2D iChannel0;

varying vec2 TexCoord;

float random(in float a, in float b) { return fract((cos(dot(vec2(a,b) ,vec2(12.9898,78.233))) * 43758.5453)); }

void main( ) {
	vec2 resolution = vec2(iResolution.xy);
	float time = iTime;

	vec2 pos = TexCoord;//( fragCoord.xy / resolution.xy );
	//pos += .01 * vec2(1. * sin(time), 1. * cos(time));
	//pos *= 3.;
	
	vec3 oricol = texture2D(iChannel0, vec2(pos.x,pos.y)).xyz;
    vec3 col;

    col.r = texture2D(iChannel0, vec2(pos.x+0.015*sin(0.02*time),pos.y)).x;
    col.g = texture2D(iChannel0, vec2(pos.x+0.000				,pos.y)).y;
    col.b = texture2D(iChannel0, vec2(pos.x-0.015*sin(0.02*time),pos.y)).z;	
	
	float c = 1.;
	//c += sin(pos.x * 20.01);
	
	c += 2. * sin(time * 4. + pos.y * 1000.);
	c += 1. * sin(time * 1. + pos.y * 800.);
	c += 20. * sin(time * 10. + pos.y * 90000.);
	
	c += 1. * cos(time * 1. + pos.x * 1.);
	//c=1.0;
	//vignetting
	c *= sin(pos.x*3.15);
	c *= sin(pos.y*3.);
	c *= .9;
	
	pos += time;
	
	float r = random(pos.x, 	pos.y);
	float g = random(pos.x * 9., 	pos.y * 9.);
	float b = random(pos.x * 3., 	pos.y * 3.);
	
	gl_FragColor = vec4(col.x * r*c*.35, col.y * b*c*.95, col.z * g*c*.35, 1);
	//gl_FragColor = vec4(col.x , col.y , col.z , 1);
//	gl_FragColor = vec4(oricol.y, oricol.y, oricol.z, 1);
}
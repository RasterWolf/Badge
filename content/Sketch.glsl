/* 
    Author: Daniel Taylor
	License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

	Tried my hand at a sketch-looking shader.

	I'm sure that someone has used this exact method before, but oh well. I like to 
	think that this one is very readable (aka I'm not very clever with optimizations).
	There's little noise in the background, which is a good sign, however it's easy to
	create a scenerio that tricks it (the 1961 Commerical video is a good example).
	Also, text (or anything thin) looks really bad on it, don't really know how to fix
	that.

	Also, if the Shadertoy devs are reading this, the number one feature request that
	I have is a time slider. Instead of waiting for the entire video to loop back to
	the end, be able to fast forward to a specific part. It'd really help, I swear.

	Previous work:
	https://www.shadertoy.com/view/XtVGD1 - the grandaddy of all sketch shaders, by flockaroo
*/
#version 330 core

precision mediump float;

uniform float iTime;
uniform vec2 iResolution;
uniform sampler2D iChannel0;
//uniform sampler2D iChannel1;
in vec2 TexCoord;

#define PI2 6.28318530717959

#define RANGE 1.
#define STEP 1.
#define ANGLENUM 1.

#define MAGIC_GRAD_THRESH 0.01

#define MAGIC_SENSITIVITY     2.
#define MAGIC_COLOR           0.1

//---------------------------------------------------------
// Your usual image functions and utility stuff
//---------------------------------------------------------
vec4 getCol(vec2 pos)
{
    vec2 uv = pos / iResolution.xy;
    return texture2D(iChannel0, uv);
}

float getVal(vec2 pos)
{
    vec4 c=getCol(pos);
    return dot(c.xyz, vec3(0.2126, 0.7152, 0.0722));
}

vec2 getGrad(vec2 pos, float eps)
{
   	vec2 d=vec2(eps,0);
    return vec2(
        getVal(pos+d.xy)-getVal(pos-d.xy),
        getVal(pos+d.yx)-getVal(pos-d.yx)
    )/eps/2.;
}

void pR(inout vec2 p, float a) {
	p = cos(a)*p + sin(a)*vec2(p.y, -p.x);
}
float absCircular(float t)
{
    float a = floor(t + 0.5);
    return mod(abs(a - t), 1.0);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 pos = fragCoord;
    float weight = 1.0;
    
    for (float j = 0.; j < ANGLENUM; j += 1.)
    {
        vec2 dir = vec2(1, 0);
        pR(dir, j * PI2 / (2. * ANGLENUM));
        
        vec2 grad = vec2(-dir.y, dir.x);
        
        for (float i = -RANGE; i <= RANGE; i += STEP)
        {
            vec2 pos2 = pos + normalize(dir)*i;
            
            // video texture2D wrap can't be set to anything other than clamp  (-_-)
            if (pos2.y < 0. || pos2.x < 0. || pos2.x > iResolution.x || pos2.y > iResolution.y)
                continue;
            
            vec2 g = getGrad(pos2, fract(iTime/2.0)+1.0); //1.0);
            if (length(g) < MAGIC_GRAD_THRESH)
                continue;
            
            weight -= pow(abs(dot(normalize(grad), normalize(g))), MAGIC_SENSITIVITY) / floor((2. * RANGE + 1.) / STEP) / ANGLENUM;
        }
    }
    
#ifndef GRAYSCALE
    vec4 col = getCol(pos);
#else
    vec4 col = vec4(getVal(pos));
#endif
    
    vec4 background = mix(col, vec4(1), MAGIC_COLOR);
    
    // I couldn't get this to look good, but I guess it's almost obligatory at this point...
    /*float distToLine = absCircular(fragCoord.y / (iResolution.y/8.));
    background = mix(vec4(0.6,0.6,1,1), background, smoothstep(0., 0.03, distToLine));*/
    
    
    // because apparently all shaders need one of these. It's like a law or something.
    float r = length(pos - iResolution.xy*.5) / iResolution.x;
    float vign = 1. - r*r*r;
    
    vec4 a = vec4(1.0); //texture2D(iChannel1, pos/iResolution.xy);
    
    fragColor = vign * mix(vec4(0), background, weight) + a.xxxx/25.;
    //fragColor = getCol(pos);
}

void main()
{
	vec2 uv =  TexCoord.xy; 
	vec2 fragCoord = TexCoord.xy * iResolution.xy;
	vec4 fragColor = vec4(0);
	mainImage( fragColor, fragCoord );
	gl_FragColor = vec4(fragColor.rgb,1.0);
}
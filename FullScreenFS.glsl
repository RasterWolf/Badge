//#version 100
//precision mediump float;
varying vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    //gl_FragColor = vec4(TexCoord.x,TexCoord.y,0,1); //texture(ourTexture, TexCoord);
    gl_FragColor = texture2D(ourTexture, TexCoord.xy);
}

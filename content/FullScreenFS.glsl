#version 330 core
//precision mediump float;
in vec2 TexCoord;


uniform sampler2D ourTexture;

void main()
{
    //gl_FragColor = vec4(TexCoord.x,TexCoord.y,0,1); //texture(ourTexture, TexCoord);
    gl_FragColor = vec4(texture2D(ourTexture, TexCoord.xy).xyz,1.0);
}

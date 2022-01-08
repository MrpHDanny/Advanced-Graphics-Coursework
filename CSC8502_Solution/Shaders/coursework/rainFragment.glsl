#version  330  core

uniform  sampler2D  diffuseTex;

in  Vertex    {
 vec2  texCoord;
 } IN;

out  vec4  fragColour;

void  main(void)    {
//	fragColour   = vec4(0,0,1,0);
	
	fragColour   = texture(diffuseTex , IN.texCoord );
}
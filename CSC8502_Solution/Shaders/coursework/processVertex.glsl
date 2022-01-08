#version  330  core

uniform  mat4  modelMatrix;
uniform  mat4  viewMatrix;
uniform  mat4  projMatrix;
uniform  mat4  textureMatrix;

uniform float time;

// Effects 
uniform bool  shake;

in   vec3  position;
in   vec2  texCoord;
out  Vertex   {

	 vec2  texCoord;
} OUT;

void  main(void)    {

	mat4  mvp         = projMatrix * viewMatrix * modelMatrix;

	       
	gl_Position     = mvp * vec4(position , 1.0);

	// Shake
	float strength = 0.001;
	gl_Position.x += cos(time) * strength;        
	gl_Position.y += cos(time) * strength; 
	
	OUT.texCoord    = (textureMatrix * vec4(texCoord , 0.0,  1.0)). xy;
}
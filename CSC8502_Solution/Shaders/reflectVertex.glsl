#version  330  core

uniform  mat4  modelMatrix;
uniform  mat4  viewMatrix;
uniform  mat4  projMatrix;
uniform  mat4  textureMatrix;
uniform float time;

in   vec3  position;
in   vec3  normal;
in   vec2  texCoord;

out  Vertex {
	vec3  colour;
	vec2  texCoord;
	vec3  normal;
	vec3  worldPos;
} OUT;

void  main(void)
{
	mat3  normalMatrix = transpose(inverse(mat3(modelMatrix )));
	OUT.texCoord  = (textureMatrix * vec4(texCoord , 0.0,  1.0)).xy;

	OUT.normal = normalize(normalMatrix * normalize(normal));

	vec4  worldPos = (modelMatrix * vec4(position, 1));

	OUT.worldPos = worldPos.xyz;

	// Waves
    vec3 wavePos = position;
	float waveSize = 20;
    wavePos.y = (sin(wavePos.x + time) * waveSize + cos(wavePos.z + time) * waveSize) + worldPos.y + time;

	// ~ Waves


	gl_Position = (projMatrix * viewMatrix) * vec4(wavePos, 1.0);
}























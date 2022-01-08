#include "Terrain.h"
#include "../nclgl/OGLRenderer.h"

Terrain::Terrain(OGLRenderer& rend)
{
	this->r = &rend;

	// Load terrain heightmap
	terrainHeightMap = new HeightMap(TEXTUREDIR"coursework/islandmap.PNG");

	// Load textures
	grass = SOIL_load_OGL_texture(TEXTUREDIR"coursework/grass.PNG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	sand = SOIL_load_OGL_texture(TEXTUREDIR"coursework/sand.PNG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	stone = SOIL_load_OGL_texture(TEXTUREDIR"coursework/stone.PNG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	// Load map textures
	grassMap = SOIL_load_OGL_texture(TEXTUREDIR"coursework/grassmap.PNG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	sandMap = SOIL_load_OGL_texture(TEXTUREDIR"coursework/sandmap.PNG ", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	stoneMap = SOIL_load_OGL_texture(TEXTUREDIR"coursework/cliffmap.PNG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	// Load normal maps
	grassNormal = SOIL_load_OGL_texture(TEXTUREDIR"coursework/grassnormal.PNG ", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	sandNormal = SOIL_load_OGL_texture(TEXTUREDIR"coursework/sandnormal.PNG ", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	stoneNormal = SOIL_load_OGL_texture(TEXTUREDIR"coursework/stonenormal.PNG ", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	// Compile shader for terrain
	shader = new Shader(SHADERDIR"coursework/terrainVertex.glsl", SHADERDIR"coursework/terrainFragment.glsl");
	//shader = new Shader(SHADERDIR"shadowscenevert.glsl", SHADERDIR"shadowscenefrag.glsl");

	Vector3 heightmapSize = terrainHeightMap->GetHeightmapSize();
	//light = new Light(heightmapSize * Vector3(0.5f, 1.5f, 0.5f), Vector4(1, 1, 1, 1), heightmapSize.x * 2.0f);

}

Terrain::~Terrain()
{
	delete light;
	delete shader;
	delete terrainHeightMap;
}

void Terrain::Draw(float dt, GLuint shadowTex)
{
	r->BindShader(shader);
	r->UpdateShaderMatrices();

	r->SetTextureRepeating(grass, true);
	r->SetTextureRepeating(sand, true);
	r->SetTextureRepeating(stone, true);

	r->SetTextureRepeating(grassNormal, true);
	r->SetTextureRepeating(sandNormal, true);
	r->SetTextureRepeating(stoneNormal, true);


	r->SetShaderLight(*light);

	// Get uniform locations
	GLuint	grassTexLoc = glGetUniformLocation(shader->GetProgram(), "grassTex");
	GLuint	sandTexLoc = glGetUniformLocation(shader->GetProgram(), "sandTex");
	GLuint	stoneTexLoc = glGetUniformLocation(shader->GetProgram(), "stoneTex");

	GLuint	grassMapLoc = glGetUniformLocation(shader->GetProgram(), "grassMap");
	GLuint	sandMapLoc = glGetUniformLocation(shader->GetProgram(), "sandMap");
	GLuint	stoneMapLoc = glGetUniformLocation(shader->GetProgram(), "stoneMap");

	GLuint grassNormalLoc = glGetUniformLocation(shader->GetProgram(), "grassNormal");
	GLuint sandNormalLoc = glGetUniformLocation(shader->GetProgram(), "sandNormal");
	GLuint stoneNormalLoc = glGetUniformLocation(shader->GetProgram(), "stoneNormal");


	glUniform1i(grassTexLoc, 0);
	glUniform1i(sandTexLoc, 1);
	glUniform1i(stoneTexLoc, 2);

	glUniform1i(grassMapLoc, 3);
	glUniform1i(sandMapLoc, 4);
	glUniform1i(stoneMapLoc, 5);
	
	glUniform1i(grassNormalLoc, 6);
	glUniform1i(sandNormalLoc, 7);
	glUniform1i(stoneNormalLoc, 8);
	//glUniform1i(glGetUniformLocation(shader->GetProgram(), "shadowTex"), 9);

	glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
	glBindTexture(GL_TEXTURE_2D, grass);

	glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
	glBindTexture(GL_TEXTURE_2D, sand);
	
	glActiveTexture(GL_TEXTURE0 + 2); // Texture unit 2
	glBindTexture(GL_TEXTURE_2D, stone);

	glActiveTexture(GL_TEXTURE0 + 3); // Texture unit 3
	glBindTexture(GL_TEXTURE_2D, grassMap);

	glActiveTexture(GL_TEXTURE0 + 4); // Texture unit 4
	glBindTexture(GL_TEXTURE_2D, sandMap);

	glActiveTexture(GL_TEXTURE0 + 5); // Texture unit 5
	glBindTexture(GL_TEXTURE_2D, stoneMap);

	glActiveTexture(GL_TEXTURE0 + 6); // Texture unit 6
	glBindTexture(GL_TEXTURE_2D, grassNormal);

	glActiveTexture(GL_TEXTURE0 + 7); // Texture unit 7
	glBindTexture(GL_TEXTURE_2D, sandNormal);

	glActiveTexture(GL_TEXTURE0 + 8); // Texture unit 8
	glBindTexture(GL_TEXTURE_2D, stoneNormal);

	//glActiveTexture(GL_TEXTURE0 + 9);
	//glBindTexture(GL_TEXTURE_2D, shadowTex);

	terrainHeightMap->Draw();
}
#include "Rain.h"

Rain::Rain(OGLRenderer& rend)
{
	this->r = &rend;
	waterTex = SOIL_load_OGL_texture(TEXTUREDIR"coursework/water.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	mesh = Mesh::GenerateRandomPositions(50000);
	shader = new Shader("coursework/rainVertex.glsl", "coursework/rainFragment.glsl", "coursework/rainParticle.glsl");
}

Rain::~Rain()
{
	delete shader;
}

void Rain::Draw(float dt, GLuint shadowTex)
{
	startTime += dt;
	if (startTime > 10000000)
		startTime = 0;

	r->BindShader(shader);
	
	r->modelMatrix.ToIdentity();
	r->textureMatrix.ToIdentity();

	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterTex);

	glUniform1f(glGetUniformLocation(shader->GetProgram(),
		"time"), startTime);



	r->UpdateShaderMatrices();
	
	mesh->Draw();
}
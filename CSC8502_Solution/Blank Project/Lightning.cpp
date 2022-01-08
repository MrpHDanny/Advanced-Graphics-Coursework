#include "Lightning.h"

Lightning::Lightning(OGLRenderer& rend)
{
	this->r = &rend;
	lightningTex = SOIL_load_OGL_texture(TEXTUREDIR"coursework/lightning.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	mesh = Mesh::GenerateRandomLightningPositions(1);
	shader = new Shader("coursework/lightningVertex.glsl", "coursework/lightningFragment.glsl", "coursework/lightningBox.glsl");
}

Lightning::~Lightning()
{
	delete shader;
}

void Lightning::Draw(float dt, GLuint shadowTex)
{
	time += dt;
	if (time < lightningCooldown)
	{
		return;
	}
	else if (!posGenerated)
	{
		mesh = Mesh::GenerateRandomLightningPositions(1);
		posGenerated = true;
	}

	if (time > lightningCooldown + lightningDuration)
	{
		time = 0;
		posGenerated = false;
		return;
	}


	r->BindShader(shader);

	r->modelMatrix.ToIdentity();
	r->textureMatrix.ToIdentity();

	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lightningTex);

	glUniform1f(glGetUniformLocation(shader->GetProgram(),
		"time"), time);

	r->UpdateShaderMatrices();

	mesh->Draw();


}
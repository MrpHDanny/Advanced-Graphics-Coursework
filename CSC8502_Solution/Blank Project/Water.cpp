#include "Water.h"


Water::Water(OGLRenderer& rend, Camera& c)
{
	quad = Mesh::GenerateFlatTerrain(850);
	camera = &c;
	this->r = &rend;
	waterRotate = 0.0f;
	waterCycle = 0.0f;
	waterTex = SOIL_load_OGL_texture(TEXTUREDIR"coursework/water.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	//cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"sky_left.png", TEXTUREDIR"sky_right.png", TEXTUREDIR"sky_up.png", TEXTUREDIR"sky_down.png", TEXTUREDIR"sky_front.png", TEXTUREDIR"sky_back.png", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
	cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"darksky_left.png", TEXTUREDIR"darksky_right.png", TEXTUREDIR"darksky_up.png", TEXTUREDIR"darksky_down.png", TEXTUREDIR"darksky_front.png", TEXTUREDIR"darksky_back.png", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	shader = new Shader("reflectVertex.glsl", "reflectFragment.glsl");
	r->SetTextureRepeating(waterTex, true);
}

Water::~Water()
{
	delete quad;
	delete shader;
}


void Water::Draw(float dt, GLuint shadowTex)
{
	Matrix4 temp = r->textureMatrix;
	glDisable(GL_CULL_FACE);
	if (increaseTime)
	{
		startTime += dt;
		if (startTime >= 20)
			increaseTime = !increaseTime;
	}
	else
	{
		startTime -= dt;
		if (startTime <= 0)
			increaseTime = !increaseTime;
	}

	waterRotate += dt * 1.0f;
	waterCycle += dt * 0.3f;

	r->BindShader(shader);
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "cubeTex"), 2);
	glUniform1f(glGetUniformLocation(shader->GetProgram(), "time"), startTime);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterTex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);


	r->textureMatrix = Matrix4::Translation(Vector3(waterCycle, 0.0f, waterCycle)) * Matrix4::Scale(Vector3(10, 10, 10)) * Matrix4::Rotation(waterRotate, Vector3(0, 0, 1));
	r->UpdateShaderMatrices();
	quad->Draw();

	glEnable(GL_CULL_FACE);

	r->textureMatrix = temp;
}
#include "SkyBox.h"



SkyBox::SkyBox(OGLRenderer& rend, Camera& c)
{
	shader = new  Shader("skyboxVertex.glsl", "skyboxFragment.glsl");
	this->r = &rend;
	camera = &c;
	quad = Mesh::GenerateQuad();
	
	/* 
		The grey clouds are hard to see in the water reflection, so it's possible to use this different cubemap texture instead to see the reflections.
		This same line must also be uncommented in the water class.
	*/
	//cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"sky_left.png", TEXTUREDIR"sky_right.png", TEXTUREDIR"sky_up.png", TEXTUREDIR"sky_down.png", TEXTUREDIR"sky_front.png", TEXTUREDIR"sky_back.png", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
	
	cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"darksky_left.png", TEXTUREDIR"darksky_right.png", TEXTUREDIR"darksky_up.png", TEXTUREDIR"darksky_down.png", TEXTUREDIR"darksky_front.png", TEXTUREDIR"darksky_back.png", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
}

SkyBox::~SkyBox()
{
	delete quad;
	delete shader;
}

void SkyBox::Draw(float dt, GLuint shadowTex)
{
	glDepthMask(GL_FALSE);

	r->BindShader(shader);
	r->UpdateShaderMatrices();

	quad->Draw();

	glDepthMask(GL_TRUE);
}

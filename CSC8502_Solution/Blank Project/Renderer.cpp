#include "Renderer.h"
#include "../nclgl/camera.h"
#include "../nclgl/HeightMap.h"

#define SHADOWSIZE 2048

Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{

	// Nodes
	root = new SceneNode();
	terrainNode = new Terrain(*this);
	
	camera = new  Camera(-45.0f, 0.0f, terrainNode->terrainHeightMap->GetHeightmapSize() * Vector3(0.0f, 0.1f, 0.0f), *this);

	waterNode = new Water(*this, *camera);
	skyboxNode = new SkyBox(*this, *camera);
	treeNode = new Tree(*this);
	rainNode = new Rain(*this);
	fernNode = new Fern(*this);
	lightningNode = new Lightning(*this);
	characterNode = new Character(*this);

	root->AddChild(skyboxNode);
	root->AddChild(terrainNode);
	root->AddChild(rainNode);
	root->AddChild(treeNode);
	root->AddChild(fernNode);
	root->AddChild(waterNode);
	root->AddChild(characterNode);
	root->AddChild(lightningNode);


	Vector3 heightmapSize = terrainNode->terrainHeightMap->GetHeightmapSize();
	light = new Light(heightmapSize * Vector3(0.25f, 2.0f, 0.25f), Vector4(1, 1, 1, 1), heightmapSize.x * 5.0f);

	terrainNode->setLight(light);

	sceneShader = new  Shader("TexturedVertex.glsl", "TexturedFragment.glsl");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glCullFace(GL_BACK);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// ~ Post-processing ~
		blurQuad = Mesh::GenerateQuad();
		blurShader = new Shader(SHADERDIR"coursework/processVertex.glsl", "processfrag.glsl");

		// Generate  our  scene  depth  texture ...
		glGenTextures(1, &bufferDepthTex);
		glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

		//And  our  colour  texture ...
		for (int i = 0; i < 2; ++i)
		{
			glGenTextures(1, &bufferColourTex[i]);
			glBindTexture(GL_TEXTURE_2D, bufferColourTex[i]);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		}

		glGenFramebuffers(1, &bufferFBO); 
		glGenFramebuffers(1, &processFBO);

		glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !bufferDepthTex || !bufferColourTex[0])
		{
			return;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glEnable(GL_DEPTH_TEST);
		// ~ Post-processing ~

	init = true;
}

Renderer::~Renderer(void)
{
	delete camera;
	delete root;
	delete blurQuad;
	delete sceneShader;
	delete light;
}

void Renderer::UpdateScene(float dt)
{
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
}

void Renderer::RenderScene(float dt)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	RenderMainScene(dt);
	DrawPostProcess();
	PresentScene(dt);
}


void Renderer::RenderMainScene(float dt)
{
	// ~ Post-processing ~
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// ~ Post-processing ~

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	viewMatrix = camera->BuildViewMatrix();
	glViewport(0, 0, width, height);


	UpdateShaderMatrices();

	for (vector <SceneNode*>::const_iterator it = root->GetChildIteratorStart(); it != root->GetChildIteratorEnd(); ++it)
	{
		(*it)->Draw(dt, shadowTex);
	}

	// ~ Post-processing ~
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// ~ Post-processing ~
}


/* Creates a blur and screen shake effect */
void Renderer::DrawPostProcess()
{
	glBindFramebuffer(GL_FRAMEBUFFER, processFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	BindShader(blurShader);
	modelMatrix.ToIdentity();
	viewMatrix.ToIdentity();
	projMatrix.ToIdentity();
	UpdateShaderMatrices();

	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(blurShader->GetProgram(), "sceneTex"), 0);
	glUniform1f(glGetUniformLocation(blurShader->GetProgram(), "time"), elapsedTime);

	for (int i = 0; i < POST_PASSES; ++i)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
		glUniform1i(glGetUniformLocation(blurShader->GetProgram(), "isVertical"), 0);

		glBindTexture(GL_TEXTURE_2D, bufferColourTex[0]);
		blurQuad->Draw();

		//Now to swap  the  colour  buffers , and do the  second  blur  pass
		glUniform1i(glGetUniformLocation(blurShader->GetProgram(), "isVertical"), 1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);

		glBindTexture(GL_TEXTURE_2D, bufferColourTex[1]);
		blurQuad->Draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::PresentScene(float dt)
{

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	BindShader(sceneShader);

	modelMatrix.ToIdentity();
	viewMatrix.ToIdentity();
	projMatrix.ToIdentity();

	UpdateShaderMatrices();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bufferColourTex[0]);
	glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "diffuseTex"), 0);
	blurQuad->Draw();

	// Draw mini view
	glViewport(width - width / 3, height - height / 3, width /3, height / 3);
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	viewMatrix = Matrix4::BuildViewMatrix(Vector3(0, 2000, 0), Vector3(4088.0f, 0.0f, 4088.0f), Vector3(0.0f, 1.0f, 0.0f));
	UpdateShaderMatrices();
	glClear(GL_DEPTH_BUFFER_BIT);
	for (vector <SceneNode*>::const_iterator it = root->GetChildIteratorStart(); it != root->GetChildIteratorEnd(); ++it)
	{
		(*it)->Draw(dt, shadowTex);
	}
}
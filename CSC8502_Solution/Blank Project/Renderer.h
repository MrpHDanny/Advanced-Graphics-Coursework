#pragma once
#include "../nclgl/OGLRenderer.h":
#include "Terrain.h"
#include "Water.h"
#include "SkyBox.h"
#include "Character.h"
#include "Tree.h"
#include "Fern.h"
#include "Rain.h"
#include "Lightning.h"
#include "../nclgl/Light.h"
#include "../nclgl/SceneNode.h"
class HeightMap;
class Camera;

class Renderer : public OGLRenderer
{
public:
	Renderer(Window& parent);
	~Renderer(void);

	void RenderScene(float dt)	override;
	void RenderShadowScene(float dt);
	void DrawPostProcess();
	void PresentScene(float dt);
	void RenderMainScene(float dt);
	void UpdateScene(float dt)	override;
	void incrementPostPasses() { if (POST_PASSES < 10) POST_PASSES++; }
	void decrementPostPasses() { if (POST_PASSES > 0) POST_PASSES--; }
	void setElapsedTime(float time) { elapsedTime = time; }
protected:
	int POST_PASSES = 0;

	float elapsedTime = 0;
	// Scene Nodes
	SceneNode* root;
	Terrain* terrainNode;
	Water* waterNode;
	SkyBox* skyboxNode;
	Tree* treeNode;
	Fern* fernNode;
	Rain* rainNode;
	Lightning* lightningNode;
	Character* characterNode;

	Shader* blurShader;
	Mesh* blurQuad;
	GLuint bufferFBO;
	GLuint processFBO;
	GLuint bufferDepthTex;
	GLuint bufferColourTex[2];

	GLuint shadowTex;
	GLuint shadowFBO;

	Shader* sceneShader;
	Camera* camera;
	GLuint terrainTex;

	Light* light;
	
};


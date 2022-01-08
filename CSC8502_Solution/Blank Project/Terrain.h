#pragma once

#include "../nclgl/Mesh.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/HeightMap.h"
class OGLRenderer;

class Terrain : public SceneNode
{
public:
	Terrain(OGLRenderer& r);
	~Terrain();

	virtual void Draw(float dt, GLuint shadowTex);
	void setLight(Light* l) { light = l; };
	void SetShader(Shader* s) override { shader = s; };
	HeightMap* terrainHeightMap;
	GLuint grassMap;

protected:
	OGLRenderer* r;
	Shader* shader;
	Light* light;

	// Height maps

	// Terrain textures
	GLuint stone;
	GLuint grass;
	GLuint sand;

	// Terrain texture maps
	GLuint sandMap;
	GLuint stoneMap;

	// Texure normal maps
	GLuint stoneNormal;
	GLuint grassNormal;
	GLuint sandNormal;
};
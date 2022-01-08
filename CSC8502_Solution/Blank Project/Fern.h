#pragma once

#include "../nclgl/Mesh.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/MeshMaterial.h"
#include "../nclgl/Heightmap.h"

/* Fern model
https://www.cgtrader.com/items/641837/download-page
*/
class Fern : public SceneNode
{
public:
	Fern(OGLRenderer& rend);
	~Fern();

	virtual void Draw(float dt, GLuint shadowTex);

	void setLight(Light* l) { light = l; };
	void SetShader(Shader* s) override { shader = s; };

protected:
	OGLRenderer* r;
	Shader* shader;
	Mesh* mesh;
	MeshMaterial* material;
	
	Light* light;
	HeightMap* terrainMap;
	vector<GLuint> matTextures;
	
	const float heightOffset = 20; // Prevent ferns from spawning above ground
	int fernCount;
	int fernPositions[1000];
	int fernRotations[1000];
};


#pragma once

#include "../nclgl/Mesh.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/MeshMaterial.h"
#include "../nclgl/Heightmap.h"
#include "../nclgl/Light.h"

/* Tree model
https://www.stlfinder.com/model/high-quality-animated-poly-tree-free-low-poly-3d-model-ZudyLSg8/1154268/
*/
class Tree : public SceneNode
{
public:
	Tree(OGLRenderer& rend);
	~Tree();

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

	const float heightOffset = 100; // Prevent trees from spawning above ground
	int treeCount;
	int treePositions[1000];
	int treeRotations[1000];
};


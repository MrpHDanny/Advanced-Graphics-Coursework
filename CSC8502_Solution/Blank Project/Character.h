#pragma once

#include "../nclgl/Mesh.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/MeshMaterial.h"
#include "../nclgl/MeshAnimation.h"
#include "../nclgl/Heightmap.h"

/* Character model
https://www.mixamo.com/#/
*/
class Character : public SceneNode
{
public:
	Character(OGLRenderer& rend);
	~Character();

	virtual void Draw(float dt, GLuint shadowTex);

	void SetShader(Shader* s) override { shader = s; };

protected:
	OGLRenderer* r;
	Shader* shader;
	Mesh* mesh;
	MeshAnimation* anim;
	MeshMaterial* material;
	Camera* camera;
	vector<GLuint> matTextures;
	HeightMap* terrainMap;

	int currentFrame;
	float frameTime;

	const float heightOffset = 20; // Prevent characters from spawning above ground
	int charCount;
	int charPositions[1000];
	int charRotations[1000];
};


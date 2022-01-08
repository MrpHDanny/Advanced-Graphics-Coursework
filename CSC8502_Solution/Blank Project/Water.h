#pragma once

#include "../nclgl/Mesh.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/OGLRenderer.h"

/* Water texture
https://www.the3rdsequence.com/texturedb/texture/44/clear+sea+water/
*/
class Water : public SceneNode
{
public:
	Water(OGLRenderer& r, Camera& c);
	~Water();


	virtual void Draw(float dt, GLuint shadowTex);
protected:
	OGLRenderer* r;
	Mesh* quad;
	Camera* camera;

	GLuint waterTex;
	GLuint cubeMap;

	float waterRotate;
	float waterCycle;
	float startTime = 0;
	bool increaseTime = true;
	Shader* shader;
};


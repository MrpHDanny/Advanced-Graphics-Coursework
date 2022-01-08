#pragma once

#include "../nclgl/Mesh.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/OGLRenderer.h"

/* Skybox assets
	https://assetstore.unity.com/packages/2d/textures-materials/sky/allsky-free-10-sky-skybox-set-146014
*/
class SkyBox : public SceneNode
{
public:
	SkyBox(OGLRenderer& r, Camera& c);
	~SkyBox();

	virtual void Draw(float dt, GLuint shadowTex);
protected:
	Mesh* quad;
	OGLRenderer* r;
	Camera* camera;
	GLuint cubeMap;
	Shader* shader;
};


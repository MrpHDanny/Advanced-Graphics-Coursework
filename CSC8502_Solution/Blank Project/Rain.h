#pragma once
#include "../nclgl/Mesh.h"
#include "../NCLGL/Camera.h"
#include "../NCLGL/SceneNode.h"

class Rain : public SceneNode {
public:
	Rain(OGLRenderer& rend);
	~Rain(void);

	virtual void Draw(float msec, GLuint shadowTex);
protected:
	OGLRenderer* r;
	Shader* shader;
	float startTime = 0;
	bool increaseTime = true;
	GLuint waterTex;
};


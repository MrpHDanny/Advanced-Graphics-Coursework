#pragma once
#include "../nclgl/Mesh.h"
#include "../NCLGL/Camera.h"
#include "../NCLGL/SceneNode.h"

/* Lightning image
https://pngtree.com/freepng/lightning-design-lightning-effect-natural-phenomenon-lightning-source_3916935.html
*/
class Lightning : public SceneNode {
public:
	Lightning(OGLRenderer& rend);
	~Lightning(void) ;

	virtual void Draw(float msec, GLuint shadowTex);
protected:
	OGLRenderer* r;
	Shader* shader;
	float time = 0;
	bool posGenerated = false;
	GLuint lightningTex;
	float lightningDuration = 0.5;
	float lightningCooldown = 5;
};


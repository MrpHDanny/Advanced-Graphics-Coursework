#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "OGLRenderer.h"
#include <vector>

class Camera
{
public:

	Camera(OGLRenderer& rend)
	{
		this->r = &rend;
		yaw = 0.0f;
		pitch = 0.0f;

		GenerateCameraTrail(10);
		position = trail_positions.at(0);
		yaw = 90;
		pitch = -12.0f;
		target_index++;
	};

	Camera(float pitch, float yaw, Vector3 position, OGLRenderer& rend)
	{
		this->r = &rend;
		this->pitch = pitch;
		this->yaw = yaw;
		this->position = position;

		GenerateCameraTrail(10);
		this->position = trail_positions.at(0);
		this->yaw = 90;
		this->pitch = -12.0f;
		target_index++;

	}

	~Camera(void) {};

	void UpdateCamera(float dt = 1.0f);
	void ManualCamera(float dt = 1.0f);
	void AutoCamera(float dt = 1.0f);

	Matrix4 BuildViewMatrix();

	Vector3 GetPosition() const { return position; }
	void SetPosition(Vector3 val) { position = val; }

	float GetYaw() const { return yaw; }
	void SetYaw(float y) { yaw = y; }

	float GetPitch() const { return pitch; }
	void SetPitch(float p) { pitch = p; }
	void GenerateCameraTrail(int points);
	float distance(Vector3 a, Vector3 b);


protected:
	OGLRenderer* r;
	const float MOUSE_SPEED = 3;
	const float MOVE_SPEED = 500;
	float yaw;
	std::vector<Vector3> trail_positions;
	float pitch;
	double theta;
	Vector3 position;
	int target_index = 0;
	bool autoMovement = true;
};


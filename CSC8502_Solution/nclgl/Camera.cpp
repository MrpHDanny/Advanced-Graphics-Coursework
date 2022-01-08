#include "Camera.h"
#include "Window.h"
#include <algorithm>
#include <math.h> 

#define PI 3.14159265

void Camera::UpdateCamera(float dt)
{
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_O) && !autoMovement) {
		autoMovement = true;
		position = trail_positions.at(0);
		yaw = 90;
		pitch = -12.0f;
		target_index=1;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_P) && autoMovement) {
		autoMovement = false;
	}

	if (autoMovement) {
		AutoCamera(dt);
	}
	else
		ManualCamera(dt);
}

void Camera::GenerateCameraTrail(int points)
{
	Vector3 point(5000, 0, 0); 
	Vector3 origin(4088, 2000, 4088);	// hardcoded map centre position - 4088
	theta = (360 / points);
	double theta_deg = theta *PI / 180;

	trail_positions.emplace_back(point + origin);
	std::cout << point + origin << std::endl;

	for (int i = 1; i < points; i++)
	{
		double _x = point.x * cos(theta_deg) - point.z * sin(theta_deg);
		double _z = point.x * sin(theta_deg) + point.z * cos(theta_deg);

		point.x = _x;
		point.z = _z;
		std::cout << point + origin << std::endl;
		trail_positions.emplace_back(point + origin);
	}
}

void Camera::ManualCamera(float dt)
{
	pitch -= (Window::GetMouse()->GetRelativePosition().y) * MOUSE_SPEED;
	yaw -= (Window::GetMouse()->GetRelativePosition().x) * MOUSE_SPEED;

	pitch = std::min(pitch, 90.0f);
	pitch = std::max(pitch, -90.0f);

	if (yaw < 0)
	{
		yaw += 360.0f;
	}
	if (yaw > 360.0f)
	{
		yaw -= 360.0f;
	}


	Matrix4 rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0));

	Vector3 forward = rotation * Vector3(0, 0, -1);
	Vector3 right = rotation * Vector3(1, 0, 0);


	float speed = MOVE_SPEED * dt;

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_W))
	{
		position += forward * speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_S))
	{
		position -= forward * speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_A))
	{
		position -= right * speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
		position += right * speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT))
	{
		position.y += speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE))
	{
		position.y -= speed;
	}

}

Matrix4 Camera::BuildViewMatrix()
{

	if (autoMovement)
	{
		return Matrix4::BuildViewMatrix(GetPosition(), Vector3(4088.0f, 0.0f, 4088.0f), Vector3(0.0f, 1.0f, 0.0f));
	}
	return
		Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) *
		Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) *
		Matrix4::Translation(-position);
};


void Camera::AutoCamera(float dt)
{
	Vector3 target = trail_positions.at(target_index);
	float dist = distance(position, target);
	
	if (dist < 10.0f)
	{
		target_index++;
		
		if (target_index == trail_positions.size())
			target_index = 0;

		target = trail_positions.at(target_index);
	}


	Vector3 direction = target - position;
	Vector3 movement = direction / dist;
	position += movement * 4.0f;
}

float Camera::distance(Vector3 a, Vector3 b)
{
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2));
}
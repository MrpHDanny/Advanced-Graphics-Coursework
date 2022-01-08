#pragma once

#include "Vector4.h"
#include "Vector3.h"

class Light
{
public:
	Light() {} // Default constructor
	Light(const  Vector3& position, const  Vector4& colour, float  radius)
	{
		this->position = position;
		this->colour = colour;
		this->radius = radius;
	}

	~Light(void) {};

	Vector3 GetPosition() const { return position; }
	Vector4 GetColour() const { return colour; }
	float GetRadius() const { return radius; }

	void SetPosition(const Vector3& val) { position = val; }
	void SetRadius(const float val) { radius = val; }
	void SetColour(const Vector4& val) { colour = val; }
	Vector3 position;


protected:
	float radius = 0;
	Vector4 colour;
};


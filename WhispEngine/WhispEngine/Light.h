
#pragma once
#include "Color.h"
#include "glmath.h"
#include "glew/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glew/glew32.lib")

struct Light
{
	Light();

	void Init();
	void SetPos(float x, float y, float z);
	void Active(bool active);
	void Render();

	Color ambient;
	Color diffuse;
	vec3 position;

	int ref;
	bool on;
};
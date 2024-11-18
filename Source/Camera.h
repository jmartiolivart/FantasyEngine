#pragma once
#include "Module.h"
#include "Globals.h"
#include "./math-library/Geometry/Frustum.h"

class Camera {
public:
	Frustum Init();
	void LookAt(float3 cameraPos, float3 target, float3 up);
private:
};
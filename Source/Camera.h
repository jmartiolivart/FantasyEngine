#pragma once
#include "Module.h"
#include "Globals.h"
#include "./math-library/Geometry/Frustum.h"
#include "./math-library/Math/MathConstants.h"
#include "./math-library/Math/float4x4.h"
#include "./math-library/Math/MathAll.h"

class Camera {
public:
	Frustum frustum;
	Frustum Init();
	float4x4 LookAt(float3 cameraPos, float3 target, float3 up);

private:
};
#include "Camera.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "./math-library/Geometry/Frustum.h"
#include "./math-library/Math/MathConstants.h"
#include "./math-library/Math/float4x4.h"
#include "./math-library/Math/MathAll.h"


Frustum Camera::Init() {

	float aspect = *(App->GetWindow()->window_width) / *(App->GetWindow()->window_height);
	
	Frustum camera;
	
	camera.type = FrustumType::PerspectiveFrustum;
	
	//Direction
	camera.pos = float3::zero;
	camera.front = -float3::unitZ;
	camera.up = float3::unitY;

	//6 planes
	camera.nearPlaneDistance = 0.1f;
	camera.farPlaneDistance = 100.0f;
	camera.verticalFov = math::pi / 4.0f;
	camera.horizontalFov = 2.f * atanf(tanf(camera.verticalFov * 0.5f) * aspect);

	float4x4 proj = camera.ProjectionMatrix();

	return camera;

}

void Camera::LookAt(float3 position, float3 target, float3 up) {

	//TODO: Do the code for LookAt function

}
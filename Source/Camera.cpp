#include "Camera.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "./math-library/Geometry/Frustum.h"


Frustum Camera::Init() {


	float aspect = (float)*(App->GetWindow()->window_width) / *(App->GetWindow()->window_height);

	frustum.type = FrustumType::PerspectiveFrustum;

	
	//Direction
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;

	//6 planes
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect);

	float4x4 proj = frustum.ProjectionMatrix();
	return frustum;

}

float4x4 Camera::LookAt(float3 position, float3 center, float3 up) {
	float3 f = (center - position).Normalized(); // Forward
	float3 s = f.Cross(up).Normalized();    // Side
	float3 u = s.Cross(f);                  // Up
	return float4x4(
		s.x, s.y, s.z, -s.Dot(position),
		u.x, u.y, u.z, -u.Dot(position),
		-f.x, -f.y, -f.z, f.Dot(position),
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

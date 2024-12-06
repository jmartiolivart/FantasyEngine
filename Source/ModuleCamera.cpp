#include "ModuleCamera.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "./math-library/Geometry/Frustum.h"


ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}



bool ModuleCamera::Init() {

	mainCamera = new Frustum();

	mainCamera->type = FrustumType::PerspectiveFrustum;
	
	//Direction
	mainCamera->pos = float3::zero;
	mainCamera->front = float3(0.0f, -1.0f, 0.0f);
	mainCamera->up = float3(1.0f, 0.0f, 0.0f);

	//6 planes
	mainCamera->nearPlaneDistance = nearPlane;
	mainCamera->farPlaneDistance = farPlane;
	mainCamera->verticalFov = math::pi / 4.0f;
	mainCamera->horizontalFov = SetHoritzontalFov(mainCamera->verticalFov);


	position = float3::zero;
	target = float3::zero;
	up = float3::unitY;

	return true;

}

float4x4 ModuleCamera::LookAt() {
	float3 f = (target - position).Normalized(); // Forward
	float3 s = f.Cross(up).Normalized();        // Side
	float3 u = s.Cross(f);                      // Up
	return float4x4(
		s.x, s.y, s.z, -s.Dot(position),
		u.x, u.y, u.z, -u.Dot(position),
		-f.x, -f.y, -f.z, f.Dot(position),
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

float ModuleCamera::SetHoritzontalFov(float verticalFov) {
	// Change the horizontal FOV to meet the new aspect ratio.
	float aspect = (float)*(App->GetWindow()->window_width) / *(App->GetWindow()->window_height);
	return 2.f * atanf(tanf(verticalFov * 0.5f) * aspect);
}

void ModuleCamera::SetAspectRatio(float newAspectRatio) {
	// Resize the window with proper ratio
	mainCamera->verticalFov = 2.f * atanf(tanf(mainCamera->horizontalFov * 0.5f) / newAspectRatio);
}

void ModuleCamera::SetPlaneDistances(float nearPlane, float farPlane) {
	//Change the vision in the middle of the game (forest, cave, ...)
	mainCamera->nearPlaneDistance = nearPlane;
	mainCamera->farPlaneDistance = farPlane;
}

float3 ModuleCamera::Position() const {
	//Returns de camera position
	return mainCamera->pos;
}

float3 ModuleCamera::Orientation() const {
	//Returns de camera orientation
	return mainCamera->front;
}

float4x4 ModuleCamera::GetProjectionMatrix() {
	float4x4 projection = mainCamera->ProjectionMatrix();
	return projection.Transposed();
}

float4x4 ModuleCamera::GetViewMatrix() {
	float4x4 view = mainCamera->ViewMatrix();
	return view.Transposed();
}

float3 ModuleCamera::GetPosition() const {
	return position;
}

float3 ModuleCamera::GetTarget() const { 
	return target;
}

float3 ModuleCamera::GetUp() const {  
	return up;
}

void ModuleCamera::SetPosition(const float3& position)
{
	this->position = position;
}

void ModuleCamera::SetTarget(const float3& target)
{
	this->target = target;
}

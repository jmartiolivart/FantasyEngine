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
	
	mainCamera->pos = float3::zero;					// Position = cordinates origin
	mainCamera->front = float3(0.0f, 0.0f, -1.0f);  // Direction camera = -Z
	mainCamera->up = float3(0.0f, 1.0f, 0.0f);		// Camera is "standing up"

	//Configuration of the 6 planes of the frustum
	mainCamera->nearPlaneDistance = nearPlane;
	mainCamera->farPlaneDistance = farPlane;
	mainCamera->verticalFov = math::pi / 4.0f;
	mainCamera->horizontalFov = SetHoritzontalFov(mainCamera->verticalFov);



	return true;

}

float4x4 ModuleCamera::LookAt() {
	float3 z = (mainCamera->pos - (mainCamera->pos + mainCamera->front)).Normalized(); // z = normalize(eye - target)
	float3 x = mainCamera->up.Cross(z).Normalized(); // x = normalize(cross(up, z))
	float3 y = z.Cross(x); // y = cross(z, x)

	return float4x4(
		x.x, x.y, x.z, -x.Dot(mainCamera->pos), 
		y.x, y.y, y.z,  y.Dot(mainCamera->pos),
		z.x, z.y, z.z, -z.Dot(mainCamera->pos),
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
	return projection;
}

float4x4 ModuleCamera::GetViewMatrix() {
	
	float4x4 view = mainCamera->ViewMatrix();
	return view.Transposed();
}

float3 ModuleCamera::GetPosition() const {
	return mainCamera->pos;
}


float3 ModuleCamera::GetUp() const {  
	return mainCamera->up;
}

void ModuleCamera::SetPosition(const float3& position)
{
	mainCamera->pos = position;
}



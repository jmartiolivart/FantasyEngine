#include "ModuleCamera.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "./math-library/Geometry/Frustum.h"
#include <iostream>


ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}



bool ModuleCamera::Init() {

	mainCamera = new Frustum();

	mainCamera->type = FrustumType::PerspectiveFrustum;
	
	mainCamera->pos = float3(0.0f, 0.0f, 0.0f);	    // Position = cordinates origin
	mainCamera->front = float3(0.0f, 0.0f, -1.0f);  // Direction camera = -Z
	mainCamera->up = float3(0.0f, 1.0f, 0.0f);		// Camera is "standing up"

	//Configuration of the 6 planes of the frustum
	mainCamera->nearPlaneDistance = nearPlane;
	mainCamera->farPlaneDistance = farPlane;
	mainCamera->verticalFov = math::pi / 4.0f;
	mainCamera->horizontalFov = SetHoritzontalFov(mainCamera->verticalFov);


	SetPosition(GetPosition() + float3(0.0f, 1.0f, 6.0f));

	return true;

}


float4x4 ModuleCamera::LookAt() {
	float3 z = (-mainCamera->front).Normalized();
	float3 x = mainCamera->up.Cross(z).Normalized();
	float3 y = z.Cross(x);	

	return float4x4(
		x.x, x.y, x.z, -x.Dot(mainCamera->pos),
		y.x, y.y, y.z, -y.Dot(mainCamera->pos),
		z.x, z.y, z.z, -z.Dot(mainCamera->pos),
		0.0f, 0.0f, 0.0f, 1.0f
	);
}



void ModuleCamera::GoUP() {
	App->camera->SetPosition(GetPosition() + float3(0.0f, -cameraSpeed * 5.0f, 0.0f));
}

void ModuleCamera::GoDOWN() {
	App->camera->SetPosition(GetPosition() + float3(0.0f, cameraSpeed * 5.0f, 0.0f));
}

void ModuleCamera::GoSTRAIGHT() {
	App->camera->SetPosition(GetPosition() + mainCamera->front * cameraSpeed * 5.0f);
}

void ModuleCamera::GoBACKWARDS() {
	App->camera->SetPosition(GetPosition() + mainCamera->front * -cameraSpeed * 5.0f);
}

void ModuleCamera::GoLEFT() {

	// Calculate the right vector
	float3 right = mainCamera->up.Cross(mainCamera->front).Normalized();
	// Move along the direction
	App->camera->SetPosition(GetPosition() + right * cameraSpeed * 5.0f);
}

void ModuleCamera::GoRIGHT() {
	// Calculate the right vector
	float3 right = mainCamera->up.Cross(mainCamera->front).Normalized();
	// Move along the direction
	App->camera->SetPosition(GetPosition() - right * cameraSpeed * 5.0f);
}

void ModuleCamera::RotateUpwards() {
	
	const float pitchAngle = 0.01f;
	float3 right = mainCamera->front.Cross(mainCamera->up).Normalized();

	float3 newFront = mainCamera->front * cos(pitchAngle) + mainCamera->up * sin(pitchAngle);
	newFront.Normalize();

	float3 newUp = right.Cross(newFront);
	newUp.Normalize();

	mainCamera->front = newFront;
	mainCamera->up = newUp;

}

void ModuleCamera::RotateBackwards() {
    
    const float pitchAngle = -0.01f;

    float3 right = mainCamera->front.Cross(mainCamera->up).Normalized();
    
	float3 newFront = mainCamera->front * cos(pitchAngle) + mainCamera->up * sin(pitchAngle);
    newFront.Normalize();

    float3 newUp = right.Cross(newFront);
    newUp.Normalize();

    mainCamera->front = newFront;
    mainCamera->up = newUp;

}



void ModuleCamera::RotateLeft() {

	const float yawAngle = -0.01f;

	// Create rotation matrix
	float cosTheta = cos(yawAngle);
	float sinTheta = sin(yawAngle);
	float3x3 rotationMatrix = {
		{cosTheta, 0.0f, sinTheta},
		{0.0f, 1.0f, 0.0f},
		{-sinTheta, 0.0f, cosTheta}
	};

	mainCamera->front = (rotationMatrix * mainCamera->front).Normalized();

}


void ModuleCamera::RotateRight() {

	const float yawAngle = 0.01f;

	// Create rotation matrix
	float cosTheta = cos(yawAngle);
	float sinTheta = sin(yawAngle);

	float3x3 rotationMatrix = {
		{cosTheta, 0.0f, sinTheta},
		{0.0f, 1.0f, 0.0f},
		{-sinTheta, 0.0f, cosTheta}
	};

	mainCamera->front = (rotationMatrix * mainCamera->front).Normalized();

}

void ModuleCamera::DragCamera(int deltaX, int deltaY)
{
	
	// Calculate the right vector
	float3 right = mainCamera->up.Cross(mainCamera->front).Normalized();

	// Move the camera along the right and up vectors
	mainCamera->pos += right * deltaX * cameraSpeed;
	mainCamera->pos -= mainCamera->up * deltaY * cameraSpeed;
}

void ModuleCamera::RotateCamera(int deltaX, int deltaY) {

	// Use RotateLeft/RotateRight for yaw
	if (deltaX > 0) {
		RotateRight();
	}
	else if (deltaX < 0) {
		RotateLeft();
	}

	// Use RotateUpwards/RotateBackwards for pitch
	if (deltaY < 0) {
		RotateBackwards();
	}
	else if (deltaY > 0) {
		RotateUpwards();
	}
}

void ModuleCamera::Zoom(int deltaX, int deltaY)
{
	//Zoom in
	if (deltaX > 0) {
		GoSTRAIGHT();
	}
	//Zoom out
	if (deltaX < 0) {
		GoBACKWARDS();
	}
}

void ModuleCamera::Zoom(int scrollDirection) {

	if (scrollDirection > 0) {
		GoSTRAIGHT();
	}
	if (scrollDirection < 0) {
		GoBACKWARDS();
	}

}

void ModuleCamera::FocusModel()
{
	float3 direction = (focusPoint - mainCamera->pos).Normalized();
	mainCamera->front = direction;
	mainCamera->pos = focusPoint - (mainCamera->front * defaultFocusDistance);
	mainCamera->up = float3(0.0f, 1.0f, 0.0f);
}

void ModuleCamera::Orbital(int deltaX, int deltaY)
{
	const float sensitivity = 0.01f;

	cameraYaw += -deltaX * sensitivity;
	cameraPitch += -deltaY * sensitivity;

	// Camera <---> Pivot distance (orbital "ray")
	float3 orbitPos = mainCamera->pos - focusPoint;
	float radius = orbitPos.Length();

	//Calculate the new camera position
	float newX = radius * cos(cameraPitch) * sin(cameraYaw);
	float newY = radius * sin(cameraPitch);
	float newZ = radius * cos(cameraPitch) * cos(cameraYaw);

	// Assign the postion
	mainCamera->pos = focusPoint + float3(newX, newY, newZ);

	// Camera looking at focus point (model)
	mainCamera->front = (focusPoint - mainCamera->pos).Normalized();
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
	return mainCamera->pos;
}

float3 ModuleCamera::Orientation() const {
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


void ModuleCamera::SetModelMatrix(const float4x4& matrix) {
	modelMatrix = matrix;
}

float4x4& ModuleCamera::GetModelMatrix() {
	return modelMatrix;
}
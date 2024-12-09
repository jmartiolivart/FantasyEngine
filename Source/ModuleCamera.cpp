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
	
	mainCamera->pos = float3::zero;					// Position = cordinates origin
	mainCamera->front = float3(0.0f, 0.0f, -1.0f);  // Direction camera = -Z
	mainCamera->up = float3(0.0f, 1.0f, 0.0f);		// Camera is "standing up"

	//Configuration of the 6 planes of the frustum
	mainCamera->nearPlaneDistance = nearPlane;
	mainCamera->farPlaneDistance = farPlane;
	mainCamera->verticalFov = math::pi / 4.0f;
	mainCamera->horizontalFov = SetHoritzontalFov(mainCamera->verticalFov);

	SetPosition(App->camera->GetPosition() + float3(0.0f, -1.0f, 0.0f));

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

void ModuleCamera::GoUP() {
	App->camera->SetPosition(GetPosition() + float3(0.0f, -cameraSpeed * 5, 0.0f));
}

void ModuleCamera::GoDOWN() {
	App->camera->SetPosition(GetPosition() + float3(0.0f, cameraSpeed * 5, 0.0f));
}

void ModuleCamera::GoSTRAIGHT() {
	App->camera->SetPosition(GetPosition() + mainCamera->front * cameraSpeed * 5);
}

void ModuleCamera::GoBACKWARDS() {
	App->camera->SetPosition(GetPosition() + mainCamera->front * -cameraSpeed * 5);
}

void ModuleCamera::GoLEFT() {

	// Calculate the right vector
	float3 right = mainCamera->up.Cross(mainCamera->front).Normalized();
	// Move along the direction
	App->camera->SetPosition(GetPosition() + right * cameraSpeed * 5);
}

void ModuleCamera::GoRIGHT() {
	// Calculate the right vector
	float3 right = mainCamera->up.Cross(mainCamera->front).Normalized();
	// Move along the direction
	App->camera->SetPosition(GetPosition() - right * cameraSpeed * 5);
}

void ModuleCamera::RotateUpwards() {
	
	//Pitch angle
	const float pitchAngle = -0.01f;

	// Calculate "right" vector
	float3 right = mainCamera->front.Cross(mainCamera->up).Normalized();

	// Create the rotation matrix
	float cosTheta = cos(pitchAngle);
	float sinTheta = sin(pitchAngle);
	float3x3 rotationMatrix = {
		{1, 0, 0},
		{0, cosTheta, -sinTheta},
		{0, sinTheta, cosTheta}
	};

	// Calculate the new front vector after rotation
	float3 newFront = (rotationMatrix * mainCamera->front).Normalized();

	// Calculate the angle between the new front vector and the up vector
	float angle = acos(newFront.Dot(mainCamera->up));

	// Limit the angle to be within a certain range (e.g., between 10 degrees and 170 degrees)
	if (angle > math::DegToRad(10.0f) && angle < math::DegToRad(170.0f)) {
		// Apply rotation matrix
		mainCamera->front = newFront;
		mainCamera->up = (rotationMatrix * mainCamera->up).Normalized();
	} // Only apply rotation if within limits

}

void ModuleCamera::RotateBackwards(){

	//Pitch angle
	const float pitchAngle = 0.01f;

	// Calculate "right" vector
	float3 right = mainCamera->front.Cross(mainCamera->up).Normalized();

	// Create the rotation matrix using pitch angle
	float cosTheta = cos(pitchAngle);
	float sinTheta = sin(pitchAngle);

	float3x3 rotationMatrix = {
		{1, 0, 0},
		{0, cosTheta, -sinTheta},
		{0, sinTheta, cosTheta}
	};

	// Calculate the new front vector after rotation
	float3 newFront = (rotationMatrix * mainCamera->front).Normalized();

	// Calculate the angle between the new front vector and the up vector
	float angle = acos(newFront.Dot(mainCamera->up));

	// Limit the angle to be within a certain range (e.g., between 10 degrees and 170 degrees)
	if (angle > math::DegToRad(10.0f) && angle < math::DegToRad(170.0f)) {
		// Apply rotation matrix
		mainCamera->front = newFront;
		mainCamera->up = (rotationMatrix * mainCamera->up).Normalized();
	} // Only apply rotation if within limits

}


void ModuleCamera::RotateLeft() {

	//Yaw angle
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

	//Yaw angle
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

void ModuleCamera::DragCamera(int oldMouseX, int oldMouseY, int currentMouseX, int currentMouseY)
{
	int deltaX = currentMouseX - oldMouseX;
	int deltaY = currentMouseY - oldMouseY;

	// Calculate the right vector
	float3 right = mainCamera->up.Cross(mainCamera->front).Normalized();

	// Move the camera along the right and up vectors
	mainCamera->pos += right * deltaX * cameraSpeed;
	mainCamera->pos -= mainCamera->up * deltaY * cameraSpeed;
}

void ModuleCamera::RotateCamera(int oldMouseX, int oldMouseY, int currentMouseX, int currentMouseY) {
	
	int deltaX = currentMouseX - oldMouseX;
	int deltaY = currentMouseY - oldMouseY;

	// Use RotateLeft/RotateRight for yaw
	if (deltaX > 0) {
		RotateRight();
	}
	else if (deltaX < 0) {
		RotateLeft();
	}

	// Use RotateUpwards/RotateBackwards for pitch
	if (deltaY > 0) {
		RotateBackwards();
	}
	else if (deltaY < 0) {
		RotateUpwards();
	}
}

void ModuleCamera::Zoom(int oldMouseX, int oldMouseY, int currentMouseX, int currentMouseY)
{
	int deltaX = currentMouseX - oldMouseX;
	int deltaY = currentMouseY - oldMouseY;

	//Zoom in
	if (deltaX > 0) {
		GoSTRAIGHT();
	}

	//Zoom out
	if (deltaX < 0) {
		GoBACKWARDS();
	}
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



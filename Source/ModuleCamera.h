#pragma once
#include "Module.h"
#include "Globals.h"
#include "./math-library/Geometry/Frustum.h"
#include "./math-library/Math/MathConstants.h"
#include "./math-library/Math/float4x4.h"
#include "./math-library/Math/MathAll.h"

class ModuleCamera : public Module {
public:

	ModuleCamera();
	~ModuleCamera();

	bool Init();
	float4x4 LookAt();

	float SetHoritzontalFov(float horitzontalFov);
	void SetAspectRatio(float aspectRatio);
	void SetPlaneDistances(float nearPlane, float farPlane);
	float3 Position() const;
	float3 Orientation() const;
	float4x4 GetProjectionMatrix();
	float4x4 GetViewMatrix();

	inline Frustum* GetCamera() { return mainCamera; };

	float3 GetPosition() const;
	float3 GetUp() const;     

	void SetPosition(const float3& position);
	

	/*----KEY INPUTS----*/
	void GoUP();
	void GoDOWN();
	void GoSTRAIGHT();
	void GoBACKWARDS();
	void GoLEFT();
	void GoRIGHT();
	void RotateUpwards();
	void RotateBackwards();
	void RotateLeft();
	void RotateRight();
	
	/*----MOUSE INPUTS----*/
	void DragCamera(int oldMouseX, int oldMouseY, int currentMouseX, int currentMouseY);
	void RotateCamera(int oldMouseX, int oldMouseY, int currentMouseX, int currentMouseY);
	void Zoom(int oldMouseX, int oldMouseY, int currentMouseX, int currentMouseY);

	float cameraSpeed = 0.01f;

private:
	float nearPlane = 0.1f;
	float farPlane = 100.0f;
	Frustum* mainCamera = nullptr;

	float3x3 rotationMatrix;


	
};
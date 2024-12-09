#pragma once
#include "Module.h"
#include "Globals.h"

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();

private:
	const Uint8 *keyboard = NULL;

	bool middleMouseDown;
	bool rightMouseDown;
	bool augmentedSpeed;
	int prevMouseX;
	int prevMouseY;
	int currentMouseX;
	int currentMouseY;
};
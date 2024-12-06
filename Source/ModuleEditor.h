#pragma once
#include "Module.h"
#include "Globals.h"
#include <vector>

class ModuleEditor : public Module {
public:

	ModuleEditor();
	~ModuleEditor();
	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	
	
private:

	float fps;
	float ms;
	std::vector<float> fps_log;
	std::vector<float> ms_log;
	bool showLogs = false;
	bool showGraphics = false;
};
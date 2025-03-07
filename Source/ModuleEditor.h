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
	bool showGeometryLogs = false;
	bool showGraphics = false;
	bool showAbout = false;
	bool fullscreen = false;
	bool resizable = false;
	bool showConfig = false;
	bool showConsole = false;

	const char* glsl_version = "#version 460";
	char buf[20];
	float f = 20.0f;
	

	
	void AboutSection(bool show);
	void OpenGitHubPage();
	
	/* Windows sections */
	void LogSection();
	void GeometryLogSection();
	void FramerateGraphSection();
	void WindowConfigurationSection();
	void HardwareInfoSection();
	void SoftwareVersionSection();
	void GeneralOptionSection();
	
	bool vsyncEnabled = false;
	bool cullingEnabled = false;
};
#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"
#include "ModuleWindow.h"

class ModuleOpenGL;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleEditor;
class ModuleDebugDraw;
class ModuleCamera;
class ModuleTexture;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

    ModuleOpenGL* GetOpenGL() { return render; }
    ModuleWindow* GetWindow() { return window; }
    ModuleInput*  GetInput() { return input; }

    //Not sure about this being public
    ModuleOpenGL* render = nullptr;
    ModuleWindow* window = nullptr;
    ModuleCamera* camera = nullptr;
    ModuleTexture* texture = nullptr;


private:

    
    ModuleInput* input = nullptr;
    ModuleEditor* editor = nullptr;
    ModuleDebugDraw* debug = nullptr;

    std::list<Module*> modules;

};

extern Application* App;

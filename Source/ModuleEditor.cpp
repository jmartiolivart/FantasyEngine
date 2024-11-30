#include "ModuleEditor.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include <GL/glew.h>
#include "./imgui-docking/imgui.h"
#include "./imgui-docking/backends/imgui_impl_sdl2.h"
#include "./imgui-docking/backends/imgui_impl_opengl3.h"
#include <io.h>


const char* glsl_version = "#version 460";

ModuleEditor::ModuleEditor()
{}

ModuleEditor::~ModuleEditor()
{}


bool ModuleEditor::Init() {

	SDL_GLContext gl_context = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window, gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	return true;

}


update_status ModuleEditor::PreUpdate() {

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
	return UPDATE_CONTINUE;

}

update_status ModuleEditor::Update() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate() {

	return UPDATE_CONTINUE;
}


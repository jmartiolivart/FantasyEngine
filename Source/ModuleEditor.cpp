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
#include "log.h"
#include <chrono>

const char* glsl_version = "#version 460";
char buf[20];
float f = 20.0f;
static std::string logText;

ModuleEditor::ModuleEditor() {}

ModuleEditor::~ModuleEditor() {}

bool ModuleEditor::Init() {
    SDL_GLContext gl_context = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsClassic();

    ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}

update_status ModuleEditor::PreUpdate() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Console");
    if (ImGui::MenuItem("Logs")) {
        showLogs = !showLogs;
    }

    if (showLogs) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "This will show the output of logs the Engine has:\n");
        if (hasNewLogs()) {
            logText += getLogBuffer();
        }
        ImGui::InputTextMultiline("Log", (char*)logText.c_str(), logText.size(), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 10), ImGuiInputTextFlags_ReadOnly);
    }

    //FPS and MS 
    static auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime);
    fps = 1000.0f / duration.count();
    ms = static_cast<float>(duration.count());
    lastTime = currentTime;

    fps_log.push_back(fps);
    ms_log.push_back(ms);

    if (ImGui::MenuItem("Graphic")) {
        showGraphics = !showGraphics;
    }

    if (showGraphics) {
        char title[25];
        sprintf_s(title, 25, "Framerate %.1f", fps);
        ImGui::PlotHistogram("###framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
        sprintf_s(title, 25, "Milliseconds %0.1f", ms);
        ImGui::PlotHistogram("###milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
    }
    ImGui::End();

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

bool ModuleEditor::CleanUp() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    return true;
}
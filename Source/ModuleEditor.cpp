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
#ifdef _WIN32
#include <windows.h> 
#endif


static std::string logText;
static std::string tinyGLTFLogText;

ModuleEditor::ModuleEditor() {}

ModuleEditor::~ModuleEditor() {}

bool ModuleEditor::Init() {
    
    SDL_GLContext gl_context = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

    ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}

update_status ModuleEditor::PreUpdate() {
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    
    //Top bar options
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("About")) {
                showAbout = !showAbout;
            }
            if (ImGui::MenuItem("GitHub Page")) {
                OpenGitHubPage(); // Opens the GitHub page
            }
            if (ImGui::MenuItem("Quit", "Alt+F4")) {
                return UPDATE_STOP; // Stop the engine
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }


    //Sections main ui window
    ImVec4 newTitleColor = ImVec4(0.8f, 0.2f, 0.2f, 1.0f); // Vermell fort
    ImVec4 newTitleActiveColor = ImVec4(0.9f, 0.3f, 0.3f, 1.0f); // Vermell lleugerament més viu per al mode actiu

    ImGui::PushStyleColor(ImGuiCol_TitleBg, newTitleColor);
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, newTitleActiveColor);
    ImGui::Begin("Console");
    ImGui::PopStyleColor(2);
    AboutSection(showAbout);
    LogSection();
    GeometryLogSection();
    FramerateGraphSection();
    WindowConfigurationSection();
    HardwareInfoSection();
    SoftwareVersionSection();
    ImGui::End();

    //More possible options ?
    /*if (ImGui::CollapsingHeader("OpenGL")) {
        ImGui::Checkbox("VSync", &vsyncEnabled);
        ImGui::Checkbox("Culling", &cullingEnabled);
    }

    if (ImGui::CollapsingHeader("Input")) {
        ImGui::SliderFloat("Mouse Sensitivity", &mouseSensitivity, 0.1f, 2.0f);
    }

    if (ImGui::CollapsingHeader("Textures")) {
        ImGui::Checkbox("Mipmapping", &mipMappingEnabled);
        ImGui::Checkbox("Bilinear Filtering", &bilinearFiltering);
    }*/

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

void ModuleEditor::AboutSection(bool showAbout) {

    if (showAbout) {
        ImGui::Begin("About Fantasy Engine", &showAbout, ImGuiWindowFlags_AlwaysAutoResize);

        // Info Section
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.4f, 1.0f), "  Title: Fantasy Engine");
        ImGui::TextColored(ImVec4(0.6f, 1.0f, 0.6f, 1.0f),
            "  Description: Fantasy Engine is a videogame-engine made from scratch\n"
            "  for the master of Advanced Programming for AAA Video Games.");
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 1.0f, 1.0f), u8"  Creator: Joan Martí Olivart");

        ImGui::Separator();
        ImGui::Separator();

        // License Section
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "  MIT License");
        ImGui::TextWrapped("Permission is hereby granted, free of charge, to any person obtaining a "
            "copy of this software and associated documentation files (the \"Software\"), "
            "to deal in the Software without restriction, including without limitation "
            "the rights to use, copy, modify, merge, publish, distribute, sublicense, "
            "and/or sell copies of the Software...");

        ImGui::End();
    }

}
void ModuleEditor::OpenGitHubPage() {
    ShellExecuteA(0, "open", "https://github.com/jmartiolivart", nullptr, nullptr, SW_SHOWNORMAL);
}

void ModuleEditor::LogSection() {
    
    if (ImGui::CollapsingHeader("General logs")) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Output logs of the Engine: \n");
        if (hasNewLogs()) {
            logText += getLogBuffer();
        }
        ImGui::InputTextMultiline("Log", (char*)logText.c_str(), logText.size(), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 10), ImGuiInputTextFlags_ReadOnly);
    }
}

void ModuleEditor::GeometryLogSection() {


    if (ImGui::CollapsingHeader("tinyGLTF Logs")) {
       
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "tinyGLTF Logs:");
        if (hasNewTinyGLTFLogs()) {
            tinyGLTFLogText += getTinyGLTFLogBuffer();
        }
        ImGui::InputTextMultiline("##tinyGLTFLog", (char*)tinyGLTFLogText.c_str(), tinyGLTFLogText.size(), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 10), ImGuiInputTextFlags_ReadOnly);
    }
}

void ModuleEditor::FramerateGraphSection() {

    // FPS and MS using ImGui's DeltaTime
    ImGuiIO& io = ImGui::GetIO();

    // Calculate FPS and milliseconds per frame
    float fps = 1.0f / io.DeltaTime;
    float ms = io.DeltaTime * 1000.0f;

    // Update FPS and MS logs
    fps_log.push_back(fps);
    ms_log.push_back(ms);

    // Limit log size to prevent memory issues
    if (fps_log.size() > 1000) fps_log.erase(fps_log.begin());
    if (ms_log.size() > 1000) ms_log.erase(ms_log.begin());


    char title[25];

    if (ImGui::CollapsingHeader("Framerate and Milliseconds")) {

        sprintf_s(title, 25, "Framerate %.1f", fps);
        ImGui::PlotHistogram("###framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

        sprintf_s(title, 25, "Milliseconds %.1f", ms);
        ImGui::PlotHistogram("###milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
    }


}

void ModuleEditor::WindowConfigurationSection() {


    if (ImGui::CollapsingHeader("Window")) {
        if (ImGui::Checkbox("Fullscreen", &fullscreen)) {
            App->GetWindow()->SetFullscreen(fullscreen);
        }
        if (ImGui::Checkbox("Resizable", &resizable)) {
            App->window->setResizable(resizable);
        }
        ImGui::SliderInt("Window Width", App->GetWindow()->window_width, 800, 1920);
        ImGui::SliderInt("Window Height", App->GetWindow()->window_height, 600, 1080);
    }

}

void ModuleEditor::HardwareInfoSection() {

    if (ImGui::CollapsingHeader("Hardware Info")) {
        ImGui::Text("CPU Cores: %d", SDL_GetCPUCount());
        ImGui::Text("System RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);
        ImGui::Text("GPU Vendor: %s", glGetString(GL_VENDOR));
        ImGui::Text("GPU Renderer: %s", glGetString(GL_RENDERER));
        ImGui::Text("GPU OpenGL Version: %s", glGetString(GL_VERSION));
    }
}

void ModuleEditor::SoftwareVersionSection() {

    if (ImGui::CollapsingHeader("Software Versions")) {
        ImGui::Text("SDL Version: %d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
        ImGui::Text("Glew Version: %s", glewGetString(GLEW_VERSION));
        ImGui::Text("ImGui Version: %s", ImGui::GetVersion());
        ImGui::Text("MathGeoLib Version: 2.0");
        ImGui::Text("Tinygltf Version: 2.0");
    }

}

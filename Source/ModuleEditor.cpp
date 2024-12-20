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

    ImGui::StyleColorsClassic();

    ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}

update_status ModuleEditor::PreUpdate() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    
    
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("About")) {
                showAbout = true; // Toggle About window visibility
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



    if (ImGui::MenuItem("Geometry loading with TinyGLTF")) {
        showGeometryLogs = !showGeometryLogs;
    }

    if (showGeometryLogs) {
        ImGui::Begin("tinyGLTF Logs");
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "tinyGLTF Logs:");
        if (hasNewTinyGLTFLogs()) {
            tinyGLTFLogText += getTinyGLTFLogBuffer();
        }
        ImGui::InputTextMultiline("##tinyGLTFLog", (char*)tinyGLTFLogText.c_str(), tinyGLTFLogText.size(), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 10), ImGuiInputTextFlags_ReadOnly);
        ImGui::End();
    }

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

    if (ImGui::MenuItem("Graphic")) {
        showGraphics = !showGraphics;
    }

    if (showGraphics) {
        char title[25];

        sprintf_s(title, 25, "Framerate %.1f", fps);
        ImGui::PlotHistogram("###framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

        sprintf_s(title, 25, "Milliseconds %.1f", ms);
        ImGui::PlotHistogram("###milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
    }

    if (showAbout) {
        ImGui::Begin("About Fantasy Engine", &showAbout, ImGuiWindowFlags_AlwaysAutoResize);

        // Info Section
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.4f, 1.0f), "  Title: Fantasy Engine");
        ImGui::TextColored(ImVec4(0.6f, 1.0f, 0.6f, 1.0f),
            "  Description: Fantasy Engine is a videogame-engine made from scratch\n"
            "  for the master of Advanced Programming for AAA Video Games.");
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 1.0f, 1.0f), u8"  Creator: Joan Martí Olivart");

        // Libraries (with versions) in two columns
        ImGui::TextColored(ImVec4(0.4f, 0.6f, 1.0f, 1.0f), "  Libraries Used:");
        ImGui::Columns(2, nullptr, false);
        ImGui::TextColored(ImVec4(0.4f, 0.6f, 1.0f, 1.0f), "  SDL");
        ImGui::NextColumn();
        ImGui::Text("2.0");
        ImGui::NextColumn();
        ImGui::TextColored(ImVec4(0.4f, 0.6f, 1.0f, 1.0f), "  Glew");
        ImGui::NextColumn();
        ImGui::Text("2.1.0");
        ImGui::NextColumn();
        ImGui::TextColored(ImVec4(0.4f, 0.6f, 1.0f, 1.0f), "  MathGeoLib");
        ImGui::NextColumn();
        ImGui::Text("2.0");
        ImGui::NextColumn();
        ImGui::TextColored(ImVec4(0.4f, 0.6f, 1.0f, 1.0f), "  ImGui");
        ImGui::NextColumn();
        ImGui::Text("1.89");
        ImGui::NextColumn();
        ImGui::TextColored(ImVec4(0.4f, 0.6f, 1.0f, 1.0f), "  Tinygltf");
        ImGui::NextColumn();
        ImGui::Text("2.0");
        ImGui::Columns(1);

        // License Section
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "  MIT License");
        ImGui::TextWrapped("Permission is hereby granted, free of charge, to any person obtaining a "
            "copy of this software and associated documentation files (the \"Software\"), "
            "to deal in the Software without restriction, including without limitation "
            "the rights to use, copy, modify, merge, publish, distribute, sublicense, "
            "and/or sell copies of the Software...");

        ImGui::End();
    }

    
    if (ImGui::MenuItem("Configuration")) {
            showConfig = !showConfig;
        }

    if (showConfig) {
        // FullScreen
        if (ImGui::Checkbox("Editor Fullscreen", &fullscreen)) {
            App->GetWindow()->SetFullscreen(fullscreen);
        }

        //Window resize
        ImGui::SameLine();
        if (ImGui::Checkbox("Rezisable", &resizable))
            App->window->setResizable(resizable);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Restart to apply");
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

void ModuleEditor::OpenGitHubPage() {
    ShellExecuteA(0, "open", "https://github.com/jmartiolivart", nullptr, nullptr, SW_SHOWNORMAL);
}
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleOpenGL.h"
#include "ModuleCamera.h"
#include "SDL/include/SDL.h"
#include "imgui-docking/backends/imgui_impl_sdl2.h"

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
    LOG("Init SDL input event system");
    bool ret = true;
    SDL_Init(0);

    if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
    {
        LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
        ret = false;
    }

    return ret;
}

// Called every draw update
update_status ModuleInput::Update()
{
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
        switch (sdlEvent.type)
        {
        case SDL_DROPFILE: {
            char* dropped_filedir = sdlEvent.drop.file;
            App->GetModel()->Load(dropped_filedir);
            SDL_free(dropped_filedir);
            break;
        }
        case SDL_QUIT:
            return UPDATE_STOP;
        
        case SDL_WINDOWEVENT:
            if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                App->GetOpenGL()->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
            // Update the FOV
            break;
        
        case SDL_KEYDOWN: // Key is press

            if (sdlEvent.key.keysym.sym == SDLK_RSHIFT || sdlEvent.key.keysym.sym == SDLK_LSHIFT) {
                if (!augmentedSpeed) {
                    App->camera->cameraSpeed = App->camera->cameraSpeed * 3.0f;
                    augmentedSpeed = true;
                }
            }
            break;
        case SDL_KEYUP:
            if (sdlEvent.key.keysym.sym == SDLK_RSHIFT || sdlEvent.key.keysym.sym == SDLK_LSHIFT) {
                if (augmentedSpeed) {
                    App->camera->cameraSpeed = App->camera->cameraSpeed / 3.0f;
                    augmentedSpeed = false;
                }
            }
        case SDL_MOUSEBUTTONDOWN:

            prevMouseX = sdlEvent.button.x;
            prevMouseY = sdlEvent.button.y;

            if (sdlEvent.button.button == SDL_BUTTON_MIDDLE) {
                middleMouseDown = true;
            }
            if (sdlEvent.button.button == SDL_BUTTON_RIGHT) {
                rightMouseDown = true;
            }
            if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
                leftMouseDown = true;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (sdlEvent.button.button == SDL_BUTTON_MIDDLE) {
                middleMouseDown = false;
            }
            if (sdlEvent.button.button == SDL_BUTTON_RIGHT) {
                rightMouseDown = false;
            }
            if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
                leftMouseDown = false;
            }

            break;
        case SDL_MOUSEMOTION:

            SDL_GetMouseState(&currentMouseX, &currentMouseY);

            deltaX = currentMouseX - prevMouseX;
            deltaY = prevMouseY - currentMouseY;

            if (middleMouseDown) {
                App->camera->DragCamera(deltaX, deltaY);
            }
            if (leftMouseDown && (SDL_GetModState() & KMOD_LALT)) {
                App->camera->Orbital(deltaX, deltaY);
            }
            if (rightMouseDown && (SDL_GetModState() & KMOD_LALT || SDL_GetModState() & KMOD_RALT)) {
                App->camera->Zoom(deltaX, deltaY);

            }
            else if (rightMouseDown) {
                App->camera->RotateCamera(deltaX, deltaY);
            }
            prevMouseX = currentMouseX;
            prevMouseY = currentMouseY;

            break;

        case SDL_MOUSEWHEEL:

            App->camera->Zoom(sdlEvent.wheel.y);
            break;
        }
    }

    keyboard = SDL_GetKeyboardState(NULL);

    // Movement controls
    if (keyboard[SDL_SCANCODE_W] && rightMouseDown) {
        App->camera->GoSTRAIGHT();
    }
    if (keyboard[SDL_SCANCODE_S] && rightMouseDown) {
        App->camera->GoBACKWARDS();
    }
    if (keyboard[SDL_SCANCODE_A] && rightMouseDown) {
        App->camera->GoLEFT();
    }
    if (keyboard[SDL_SCANCODE_D] && rightMouseDown) {
        App->camera->GoRIGHT();
    }
    if (keyboard[SDL_SCANCODE_Q]) {
        App->camera->GoUP();
    }
    if (keyboard[SDL_SCANCODE_E]) {
        App->camera->GoDOWN();
    }
    if (keyboard[SDL_SCANCODE_F]) {
        App->camera->FocusModel();
    }
    // Rotation controls
   if (keyboard[SDL_SCANCODE_UP]) {
        App->camera->RotateUpwards();
    }
    if (keyboard[SDL_SCANCODE_DOWN]) {
        App->camera->RotateBackwards();
    }
    if (keyboard[SDL_SCANCODE_LEFT]) {
        App->camera->RotateLeft();
    }
    if (keyboard[SDL_SCANCODE_RIGHT]) {
        App->camera->RotateRight();
    }
    



    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
    LOG("Quitting SDL input event subsystem");
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
    return true;
}
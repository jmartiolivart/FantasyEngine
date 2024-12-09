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

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
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
                if (sdlEvent.key.keysym.sym == SDLK_q) {
                    App->camera->GoUP();
                }
                if (sdlEvent.key.keysym.sym == SDLK_e) {
                    App->camera->GoDOWN();
                }
                if (sdlEvent.key.keysym.sym == SDLK_w) {
                    App->camera->GoSTRAIGHT();
                }
                if (sdlEvent.key.keysym.sym == SDLK_s) {
                    App->camera->GoBACKWARDS();
                }
                if (sdlEvent.key.keysym.sym == SDLK_a) {
                    App->camera->GoLEFT();
                }
                if (sdlEvent.key.keysym.sym == SDLK_d) {
                    App->camera->GoRIGHT();
                }
                if (sdlEvent.key.keysym.sym == SDLK_UP) {
                    App->camera->RotateUpwards();
                }
                if (sdlEvent.key.keysym.sym == SDLK_DOWN) {
                    App->camera->RotateBackwards();
                }
                if (sdlEvent.key.keysym.sym == SDLK_LEFT) {
                    App->camera->RotateLeft();
                }
                if (sdlEvent.key.keysym.sym == SDLK_RIGHT) {
                    App->camera->RotateRight();
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
                break;
            
            case SDL_MOUSEBUTTONUP:
                if (sdlEvent.button.button == SDL_BUTTON_MIDDLE) {
                    middleMouseDown = false;
                }
                if (sdlEvent.button.button == SDL_BUTTON_RIGHT) {
                    rightMouseDown = false;
                }

                break;
            case SDL_MOUSEMOTION:

                SDL_GetMouseState(&currentMouseX, &currentMouseY);

                if (middleMouseDown) {
                    App->camera->DragCamera(prevMouseX, prevMouseY, currentMouseX, currentMouseY);
                    
                }
                if(rightMouseDown && (SDL_GetModState() & KMOD_LALT || SDL_GetModState() & KMOD_RALT)){
                    App->camera->Zoom(prevMouseX, prevMouseY, currentMouseX, currentMouseY);

                } else if (rightMouseDown) {
                    App->camera->RotateCamera(prevMouseX, prevMouseY, currentMouseX, currentMouseY);
                }
                prevMouseX = currentMouseX;
                prevMouseY = currentMouseY;

                break;
        }
    }

    keyboard = SDL_GetKeyboardState(NULL);

    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

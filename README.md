# Fantasy Engine

## About
**Fantasy Engine** is a video game engine built from scratch as part of the *Master of Advanced Programming for AAA Video Games*. It is designed using a modular approach, where each module controls a specific part of the engine.

## Modular Architecture

Below is a summary of the main classes (modules) in **Fantasy Engine**, how they fit into the overall architecture, and some of their key methods.

| **Module / Class**   | **Responsibility**                                                                                                                      | **Key Methods / Functions**                                                                                                                                                                                                                                                           |
|----------------------|----------------------------------------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| **Application**      | Orchestrates the entire engine workflow.<br>Holds a list of modules, calls their `Init()`, `Update()`, and `CleanUp()` methods in order. | - **`Application()`** – Constructor that instantiates and orders modules (Window, OpenGL, Camera, etc.)<br>- **`Init()`** – Loops through modules, calling `Init()` on each.<br>- **`Update()`** – Runs the main loop, calling `PreUpdate()`, `Update()`, and `PostUpdate()` on each.<br>- **`CleanUp()`** – Calls each module’s `CleanUp()` in reverse order. |
| **ModuleWindow**     | Creates and manages the SDL window where Fantasy Engine runs.<br>Sets OpenGL attributes before creating the window.                     | - **`Init()`** – Initializes SDL, creates window, sets up OpenGL attributes.<br>- **`CleanUp()`** – Destroys the window, quits SDL.<br>- **`SetFullscreen(bool)`** – Toggles fullscreen mode.<br>- **`setResizable(bool)`** – Enables or disables window resizing.                          |
| **ModuleOpenGL**     | Manages all core OpenGL functionality.<br>Creates and owns the OpenGL context, clears buffers, and swaps window buffers.               | - **`Init()`** – Creates the OpenGL context with SDL, initializes GLEW, compiles default shaders.<br>- **`PreUpdate()`** – Clears color/depth buffers.<br>- **`Update()`** – Renders the active model’s meshes using the current view & projection matrices.<br>- **`PostUpdate()`** – Swaps the window buffers to display the rendered frame.<br>- **`CleanUp()`** – Deletes the shader program and cleans up resources. |
| **ModuleCamera**     | Controls the main camera’s position, rotation, zoom, orbital movement, etc.<br>Generates view and projection matrices.                 | - **`Init()`** – Sets up a default perspective `Frustum`.<br>- **`LookAt()`** – Generates a view matrix based on camera orientation.<br>- **`FocusModel()`** – Positions the camera to look directly at the model.<br>- **`Orbital(int dx, int dy)`** – Orbits around a focus point.<br>- **`Zoom(int delta)`** – Moves the camera forward/back.<br>- **`GetViewMatrix()` / `GetProjectionMatrix()`** – Returns current camera matrices. |
| **ModuleTexture**    | Loads textures (via `stb_image`) and caches them in a `std::map`.<br>Handles texture parameters and cleanup.                            | - **`Init()`** – Logs texture module initialization.<br>- **`Load(const std::string&)`** – Loads a texture from file, returns its `GLuint` ID, caches it.<br>- **`CleanUp()`** – Frees all cached textures and clears the map.                                                         |
| **ModuleModel**      | Handles loading and storing of 3D models (GLTF, etc.).<br>Manages mesh data, can replace the active model on drag-and-drop.             | - **`Init()`** – Loads a default model (e.g., `BakerHouse.gltf`).<br>- **`Load(const char*)`** – Loads a new model from a file path, replaces the old one.<br>- **`GetModel()`** – Returns a pointer to the current `Model`.                                                            |
| **ModuleInput**      | Processes SDL events (mouse, keyboard, window).<br>Implements drag-and-drop, camera controls, etc.                                     | - **`Init()`** – Initializes SDL event subsystem.<br>- **`Update()`** – Polls SDL events: drag-and-drop, window resizing, camera move/rotate/orbit/zoom, keyboard shortcuts.<br>- **`CleanUp()`** – Shuts down SDL’s events subsystem.                                                  |
| **ModuleEditor**     | Creates and manages the ImGui-based UI.<br>Implements menus (File, Console, About), logs, framerate graphs, and general options.       | - **`Init()`** – Initializes ImGui with SDL/OpenGL backends.<br>- **`PreUpdate()`** – Builds the main menu bar, renders console, about window, etc.<br>- **`Update()`** – Renders all ImGui draw data.<br>- **`CleanUp()`** – Shuts down ImGui context and cleans up.                   |
| **ModuleDebugDraw**  | Uses `debug-draw` (dd::) to render debug shapes like grids and axis triads.<br>Applies an OpenGL-based `RenderInterface`.               | - **`Init()`** – Creates the `DDRenderInterfaceCoreGL`, calls `dd::initialize()`.<br>- **`PreUpdate()`** – Draws a grid, axis triad, etc.<br>- **`Update()`** – Flushes debug draw commands to the screen.<br>- **`CleanUp()`** – Calls `dd::shutdown()`.                                    |


**GitHub Repository**: [jmartiolivart/FantasyEngine](https://github.com/jmartiolivart/FantasyEngine)

---

## Features
- **Drag and drop `.gltf`** files directly into the engine.
- **Focus on a model** by pressing `F`.
- **Orbital spin** to rotate the model around using the mouse.
- **Modular structure** for organizing subsystems (graphics, input, etc.) in a maintainable way.

---

## Movement and Controls

1. **Vertical Movement**  
   - **Q / E**: Move **Up** / **Down**  
   <p align="left">
     <img src="https://github.com/user-attachments/assets/f63d8284-7733-415b-9fa8-65399ec5a98c" alt="E_Q Movement" width="300"/>
   </p>

2. **Horizontal Movement**  
   - **W / A / S / D**: Move **Forward**, **Left**, **Backward**, **Right** respectively  
   <p align="left">
     <img src="https://github.com/user-attachments/assets/0e6361d0-d412-4982-a36c-e0dfeec6cda4" alt="W_A_S_D Movement" width="300"/>
   </p>

3. **Mouse Controls**  
   - **Middle Mouse Button**: Grab/pan the scene  
   - **Alt + Right Mouse Button**: Zoom in / out  
   <p align="left">
     <img src="https://github.com/user-attachments/assets/38d84e26-3212-4e2b-9959-9d14eaa6e0f4" alt="Mouse Functions" width="300"/>
   </p>

4. **Movement Speed**  
   - **Shift**: Increases movement speed  
   <p align="left">
     <img src="https://github.com/user-attachments/assets/841c2052-e0cf-41d5-97ac-6ccc0f6f6ad4" alt="Shift Increase Speed" width="300"/>
   </p>

5. **Focus on Model**  
   - **F**: Instantly focus the camera on the selected model  
   <p align="left">
     <img src="https://github.com/user-attachments/assets/c8d34ca9-f2da-476e-b9f3-7577816f8816" alt="Focus" width="300"/>
   </p>

6. **Orbital View**  
   - Orbit around the model to inspect it from multiple angles  
   <p align="left">
     <img src="https://github.com/user-attachments/assets/3a92a4bd-5451-452a-949b-7b06ef51c39d" alt="Orbital" width="300"/>
   </p>

---

## License
This project is licensed under the terms specified in the repository.  
See the [LICENSE](https://github.com/jmartiolivart/FantasyEngine/blob/main/LICENCE.md) file for details.

---

**Enjoy using Fantasy Engine!**

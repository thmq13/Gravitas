## Overview ##
A sandbox for simulating gravitational interaction, aimed to be scalable and comprehensive.

## Software Stack ##
__Language__: C++23

__Graphics / Windowing__: raylib

__GUI__: Dear ImGui

__Binding__: rlImGui

__Build System__: CMake (3.20+)

## Project Structure ##
```
Gravitas/
├── include/           # Header files
│   ├── Core/          # Application and core systems
│   ├── Generator/     # Particle generators
│   ├── Utils/         # Math and utilities
│   ├── Particle/      # Particle data model
│   ├── Physics/       # Physics logic
│   ├── Render/        # Render pipeline
│   └── UI/            # ImGui interface layouts
├── shaders/           # GLSL shaders
└── src/               # Source files (.cpp)
```

## Prerequisites ##
Before building, ensure you have the following installed:

- CMake (v3.20 or higher)

- A C++23 compliant compiler

- Git

__Note__: External dependencies (raylib, Dear ImGui, and rlImGui) are managed automatically via CMake's FetchContent and do not need to be installed manually.

## Build and Setup ##
__1. Clone the repository__
```
Bash
git clone https://github.com/thmq13/Gravitas.git
cd Gravitas
```
__2. Configure the project__

Generate the build files using CMake:
```
Bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

__3. Build the executable__

Compile the project:
```
Bash
cmake --build build --config Release
```
__4. Run the application__
```
Bash
# Windows
.\build\bin\Release\Gravitas.exe

# Linux / macOS
./build/bin/Gravitas
```

## Controls and Usage ##
The application is currently in its early development stage.

## License ##
This project is licensed under the MIT License. See the LICENSE file for details.
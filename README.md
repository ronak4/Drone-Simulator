# Drone Simulator

## Overview
This project is a C++ application that simulates a flying drone in a 3D environment using modern OpenGL (4.x). The drone is fully animated and interactive, featuring detailed components such as a multi-part fuselage, four distinct propeller blades per fan, and four-legged landing gear. The application supports multiple camera views to help you observe the drone’s movement from various perspectives.

## Features
- **Animated Drone Model:**
    - Detailed fuselage with a clearly defined front (cockpit/nose).
    - Two propeller fans, each with four distinct blades that rotate continuously.
    - Landing gear with four legs.
- **Real-Time Animation:**
    - Propellers rotate perpetually with adjustable speed (via the 's' key for slower and the 'f' key for faster).
    - Sideways roll animation triggered by the 'j' key, performing a full 360° roll.
- **Interactive Drone Control:**
    - Move the drone forwards/backwards relative to its current facing direction; movement speed scales with the propeller speed.
    - The drone’s movement accounts for its pitch (up/down orientation) for natural flight dynamics.
- **Multiple Camera Views:**
    - **Global Camera:** Provides an overall view of the entire scene.
    - **Chopper Camera:** Rotates above the scene while continuously tracking the drone.
    - **Cockpit Camera:** Offers a first-person view from the front of the drone, moving and rotating with it.
- **3D Environment Markers:**
    - Coordinate axes at the origin.
    - Small orange squares on each wall of the enclosing room to aid spatial orientation.

## How It Works
The application is structured into several key components:
1. **Drone Model:**  
   The `Drone` class handles the creation, animation, and rendering of the drone. It manages the detailed drone parts (fuselage, propellers, landing gear) and implements continuous animations (propeller rotation and roll).
2. **Camera System:**  
   The `Camera` class provides perspective projection and view transformations. Three camera instances are created for the global, chopper, and cockpit views, with the chopper and cockpit cameras updating dynamically based on the drone’s position and orientation.
3. **Scene Management:**  
   The `Scene` class aggregates all scene elements, including the drone, cameras, coordinate markers, and wall markers. It updates and renders each component every frame.
4. **Input Handling:**  
   The `InputHandler` class maps keyboard inputs to drone movements (forwards, backwards, roll, turning, etc.) and camera switching, ensuring an interactive experience.
5. **Shader Management:**  
   The `Shader` class compiles and manages GLSL shader programmes used for rendering the 3D scene.

User inputs directly affect the drone’s behaviour and the active camera view, allowing for an immersive and interactive simulation.

## File Structure
```
├── src/
│   ├── main.cpp                   # Entry point: initialises OpenGL, the scene, and the main loop.
│   ├── Drone.h / Drone.cpp        # Defines and implements the drone model, its animation, and rendering.
│   ├── Camera.h / Camera.cpp      # Implements different camera views and updates.
│   ├── Scene.h / Scene.cpp        # Manages the scene objects including the drone, cameras, and markers.
│   ├── InputHandler.h / InputHandler.cpp  # Handles keyboard input for drone and camera control.
│   ├── Shader.h / Shader.cpp      # Compiles and manages GLSL shaders.
├── include/                       # Local project headers.
├── Makefile                       # Cross-platform build instructions.
```

## Build Instructions
To build the project:
1. Navigate to the project directory.
2. Run:
   ```bash
   make
   ```
On Windows (using a compatible environment such as MinGW), run:
   ```bash
   mingw32-make
   ```

## Usage
- **Drone Controls:**
    - **'+' / '-'**: Move the drone forwards/backwards relative to its facing direction.
    - **Arrow Keys**: Adjust the drone’s pitch and yaw.
    - **'s' / 'f'**: Decrease/Increase the propeller speed (affects both propeller animation and movement).
    - **'j'**: Initiate a full 360° roll.
    - **'d'**: Reset the drone’s position.
- **Camera Switching:**
    - **'1'**: Switch to Global Camera.
    - **'2'**: Switch to Chopper Camera.
    - **'3'**: Switch to Cockpit (First-Person) Camera.

#ifndef SCENE_H
#define SCENE_H

#include "Drone.h"
#include "Camera.h"
#include "Shader.h"
#include <vector>

class Scene {
public:
    Scene(int width, int height);

    void update();
    void render(Shader* shader);

    // Returns a pointer to the drone
    Drone* getDrone();

    // Set active camera by index: 0 - Global, 1 - Chopper, 2 - First-person.
    void setActiveCamera(int index);

    // Get the active camera.
    Camera* getActiveCamera();

private:
    Drone drone;
    std::vector<Camera*> cameras;
    int activeCameraIndex;

    int screenWidth;
    int screenHeight;

    void renderMarkers(Shader* shader);
    void renderWallMarkers(Shader* shader);
};

#endif // SCENE_H

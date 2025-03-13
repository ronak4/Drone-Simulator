#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <GLFW/glfw3.h>
#include "Scene.h"

class InputHandler {
public:
    // GLFW key callback.
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    // Set the scene pointer for input interactions.
    static void setScene(Scene* scenePtr);

private:
    static Scene* scene;
};

#endif // INPUTHANDLER_H

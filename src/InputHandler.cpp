#include "InputHandler.h"
#include "Drone.h"
#include "Camera.h"
#include <iostream>

Scene* InputHandler::scene = nullptr;

void InputHandler::setScene(Scene* scenePtr) {
    scene = scenePtr;
}

void InputHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS || action == GLFW_REPEAT) {
        if(!scene) return;
        Drone* drone = scene->getDrone();

        // Adjust propeller speed.
        if(key == GLFW_KEY_S) {
            drone->decreasePropellerSpeed();
        }
        if(key == GLFW_KEY_F) {
            drone->increasePropellerSpeed();
        }
        // Trigger roll.
        if(key == GLFW_KEY_J) {
            drone->roll();
        }
        // Move forward/backward.
        if(key == GLFW_KEY_KP_ADD || key == GLFW_KEY_EQUAL) { // '+' key.
            drone->moveForward();
        }
        if(key == GLFW_KEY_KP_SUBTRACT || key == GLFW_KEY_MINUS) { // '-' key.
            drone->moveBackward();
        }
        // Turn using arrow keys.
        if(key == GLFW_KEY_LEFT) {
            drone->turnLeft();
        }
        if(key == GLFW_KEY_RIGHT) {
            drone->turnRight();
        }
        if(key == GLFW_KEY_UP) {
            drone->turnUp();
        }
        if(key == GLFW_KEY_DOWN) {
            drone->turnDown();
        }
        // Reset the drone.
        if(key == GLFW_KEY_D) {
            drone->reset();
        }
        // Switch cameras (1, 2, 3).
        if(key == GLFW_KEY_1) {
            scene->setActiveCamera(0);
        }
        if(key == GLFW_KEY_2) {
            scene->setActiveCamera(1);
        }
        if(key == GLFW_KEY_3) {
            scene->setActiveCamera(2);
        }
    }
}

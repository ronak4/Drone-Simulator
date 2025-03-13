#ifndef DRONE_H
#define DRONE_H

#include <glm/glm.hpp>
#include "Shader.h"

class Drone {
public:
    Drone();

    // Update drone animation and state.
    void update(float deltaTime);

    // Render the drone model using the provided shader.
    void render(Shader* shader);

    // Control methods.
    void increasePropellerSpeed();
    void decreasePropellerSpeed();
    void roll(); // Trigger a 360° roll.
    void moveForward();
    void moveBackward();
    void turnLeft();
    void turnRight();
    void turnUp();
    void turnDown();
    void reset();

    // Get current position
    glm::vec3 getPosition() const;
    // Compute the front direction of the drone.
    glm::vec3 getFront() const;
    // Get current rotation
    glm::vec3 getRotation() const;

private:
    glm::vec3 position;
    glm::vec3 rotation; // rotation.x = pitch, rotation.y = yaw, rotation.z = roll

    // Propeller speed and state.
    float propellerSpeed;

    // Roll animation state.
    bool isRolling;
    float rollAngle; // Current roll angle during animation.

    float currentPropellerAngle;

    // Helper functions for rendering parts.
    void renderBody(Shader* shader);
    void renderPropeller(Shader* shader, const glm::vec3 &offset);
    void renderLandingGear(Shader* shader);

    // Utility functions to draw primitives.
    void drawCube(Shader* shader, const glm::mat4 &model, const glm::vec3 &color);
    void drawQuad(Shader* shader, const glm::mat4 &model, const glm::vec3 &color);
};

#endif // DRONE_H

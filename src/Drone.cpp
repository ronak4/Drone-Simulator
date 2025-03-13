#include "Drone.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

// Static variables for cube geometry.
static unsigned int cubeVAO = 0, cubeVBO = 0;
static unsigned int quadVAO = 0, quadVBO = 0;

// Vertex data for a cube
static float cubeVertices[] = {
        // positions
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
};

// Vertex data for a quad
static float quadVertices[] = {
        // positions
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f,  0.5f, 0.0f,

        0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
};

static void initCube() {
    if(cubeVAO != 0)
        return;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

static void initQuad() {
    if(quadVAO != 0)
        return;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Drone::Drone() : position(0.0f, 2.0f, 0.0f), rotation(0.0f),
                 propellerSpeed(100.0f), isRolling(false), rollAngle(0.0f), currentPropellerAngle(0.0f)
{
    // Ensure geometry is initialised.
    initCube();
    initQuad();
}

void Drone::update(float deltaTime) {
    // Update continuous propeller rotation.
    currentPropellerAngle += propellerSpeed * deltaTime;
    if (currentPropellerAngle > 360.0f)
        currentPropellerAngle = fmod(currentPropellerAngle, 360.0f);

    // Update roll animation if triggered.
    if (isRolling) {
        float rollSpeed = 180.0f; // degrees per second for roll animation.
        rollAngle += rollSpeed * deltaTime;
        if (rollAngle >= 360.0f) {
            rollAngle = 0.0f;
            isRolling = false;
        }
    }
}


void Drone::render(Shader* shader) {
    // Render the drone parts using the shader.
    renderBody(shader);
    renderPropeller(shader, glm::vec3(1.0f, 0.5f, 0.0f));  // Right propeller.
    renderPropeller(shader, glm::vec3(-1.0f, 0.5f, 0.0f)); // Left propeller.
    renderLandingGear(shader);
}

void Drone::increasePropellerSpeed() {
    propellerSpeed += 10.0f;
}

void Drone::decreasePropellerSpeed() {
    if(propellerSpeed > 10.0f)
        propellerSpeed -= 10.0f;
}

void Drone::roll() {
    isRolling = true;
    rollAngle = 0.0f;
}

void Drone::moveForward() {
    glm::vec3 front = getFront();
    position += front * (propellerSpeed * 0.001f);
}

void Drone::moveBackward() {
    glm::vec3 front = getFront();
    position -= front * (propellerSpeed * 0.001f);
}

void Drone::turnLeft() {
    rotation.y += 5.0f;
}

void Drone::turnRight() {
    rotation.y -= 5.0f;
}

void Drone::turnUp() {
    rotation.x += 5.0f;
}

void Drone::turnDown() {
    rotation.x -= 5.0f;
}

void Drone::reset() {
    position = glm::vec3(0.0f, 2.0f, 0.0f);
    rotation = glm::vec3(0.0f);
    rollAngle = 0.0f;
    isRolling = false;
}

glm::vec3 Drone::getPosition() const {
    return position;
}

glm::vec3 Drone::getRotation() const {
    return rotation;
}

glm::vec3 Drone::getFront() const {
    float yaw = rotation.y;
    float pitch = rotation.x;
    glm::vec3 front;
    front.x = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    return glm::normalize(front);
}


// Helper function: draw a cube given a model matrix and color.
void Drone::drawCube(Shader* shader, const glm::mat4 &model, const glm::vec3 &color) {
    shader->setMat4("model", model);
    shader->setVec3("objectColor", color);
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// Helper function: draw a quad given a model matrix and color.
void Drone::drawQuad(Shader* shader, const glm::mat4 &model, const glm::vec3 &color) {
    shader->setMat4("model", model);
    shader->setVec3("objectColor", color);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Drone::renderBody(Shader* shader) {
    // Create a base transformation using the drone's position and rotation.
    glm::mat4 baseModel = glm::mat4(1.0f);
    baseModel = glm::translate(baseModel, position);
    baseModel = glm::rotate(baseModel, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    baseModel = glm::rotate(baseModel, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    baseModel = glm::rotate(baseModel, glm::radians(rotation.z + rollAngle), glm::vec3(0, 0, 1));

    // Draw the main fuselage
    glm::mat4 fuselageModel = glm::scale(baseModel, glm::vec3(1.2f, 0.3f, 0.5f));
    drawCube(shader, fuselageModel, glm::vec3(0.2f, 0.2f, 0.8f));

    // Draw the cockpit/nose at the front
    glm::mat4 cockpitModel = glm::translate(baseModel, glm::vec3(0.0f, 0.0f, -0.5f));
    cockpitModel = glm::scale(cockpitModel, glm::vec3(0.4f, 0.2f, 0.4f));
    drawCube(shader, cockpitModel, glm::vec3(0.8f, 0.2f, 0.2f));
}

void Drone::renderPropeller(Shader* shader, const glm::vec3 &offset) {
    glm::mat4 baseModel = glm::mat4(1.0f);
    // Apply the drone's position and rotation.
    baseModel = glm::translate(baseModel, position);
    baseModel = glm::rotate(baseModel, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    baseModel = glm::rotate(baseModel, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    baseModel = glm::rotate(baseModel, glm::radians(rotation.z + rollAngle), glm::vec3(0, 0, 1));
    // Move to the propeller's offset.
    baseModel = glm::translate(baseModel, offset);

    // Rotate the propeller assembly by the continuously updated angle.
    baseModel = glm::rotate(baseModel, glm::radians(currentPropellerAngle), glm::vec3(0, 1, 0));

    // Render 4 distinct blades.
    for (int i = 0; i < 4; i++) {
        glm::mat4 bladeModel = glm::rotate(baseModel, glm::radians(i * 90.0f), glm::vec3(0, 1, 0));
        // Translate each blade outward from the center.
        bladeModel = glm::translate(bladeModel, glm::vec3(0.5f, 0.0f, 0.0f));
        // Scale to form a thin rectangular blade.
        bladeModel = glm::scale(bladeModel, glm::vec3(1.0f, 0.05f, 0.2f));
        drawCube(shader, bladeModel, glm::vec3(0.8f, 0.8f, 0.2f));
    }
}

void Drone::renderLandingGear(Shader* shader) {
    // Create a base transformation from the drone's position and rotation.
    glm::mat4 baseModel = glm::mat4(1.0f);
    baseModel = glm::translate(baseModel, position);
    baseModel = glm::rotate(baseModel, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    baseModel = glm::rotate(baseModel, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    baseModel = glm::rotate(baseModel, glm::radians(rotation.z + rollAngle), glm::vec3(0, 0, 1));

    // Define leg offsets
    // Rear legs
    glm::vec3 rearRightOffset = glm::vec3( 0.3f, -0.15f,  0.3f);
    glm::vec3 rearLeftOffset  = glm::vec3(-0.3f, -0.15f,  0.3f);
    // Front legs
    glm::vec3 frontRightOffset = glm::vec3( 0.3f, -0.15f, -0.2f);
    glm::vec3 frontLeftOffset  = glm::vec3(-0.3f, -0.15f, -0.2f);

    // Helper lambda to draw a leg and its wheel at a given offset.
    auto drawLegAndWheel = [&](const glm::vec3 &offset) {
        // Draw leg: translate by offset and scale down to a thin column.
        glm::mat4 legModel = glm::translate(baseModel, offset);
        legModel = glm::scale(legModel, glm::vec3(0.1f, 0.3f, 0.1f));
        drawCube(shader, legModel, glm::vec3(0.5f, 0.5f, 0.5f));

        // Draw wheel: translate to the same X/Z offset but lower on Y.
        glm::mat4 wheelModel = glm::translate(baseModel, glm::vec3(offset.x, -0.5f, offset.z));
        wheelModel = glm::scale(wheelModel, glm::vec3(0.15f, 0.05f, 0.15f));
        drawCube(shader, wheelModel, glm::vec3(0.1f, 0.1f, 0.1f));
    };

    // Draw all four legs.
    drawLegAndWheel(rearRightOffset);
    drawLegAndWheel(rearLeftOffset);
    drawLegAndWheel(frontRightOffset);
    drawLegAndWheel(frontLeftOffset);
}

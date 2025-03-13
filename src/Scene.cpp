#include "Scene.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

// Helper function to render a unit quad in the XY plane (z=0)
static unsigned int quadVAO = 0, quadVBO = 0;
static void initQuad()
{
    if (quadVAO != 0)
        return;
    float quadVertices[] = {
            // positions (centered quad in XY plane)
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f,  0.5f, 0.0f,
            0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f
    };
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Scene::renderWallMarkers(Shader* shader) {
    initQuad(); // ensure the quad is initialised

    shader->use();

    // Define the color for the markers (orange).
    glm::vec3 markerColor = glm::vec3(1.0f, 0.5f, 0.0f);

    // We'll use a small square (scale it by 1.0) for each marker.
    float markerScale = 1.0f;

    // Back Wall: at z = -20, centered at (0, 5, -20).
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 5.0f, -20.0f));
        model = glm::scale(model, glm::vec3(markerScale));
        shader->setMat4("model", model);
        shader->setVec3("objectColor", markerColor);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // Front Wall: at z = 20, centered at (0, 5, 20).
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 5.0f, 20.0f));
        // Rotate 180° around Y so the square faces inward.
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(markerScale));
        shader->setMat4("model", model);
        shader->setVec3("objectColor", markerColor);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // Left Wall: at x = -20, centered at (-20, 5, 0).
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-20.0f, 5.0f, 0.0f));
        // Rotate 90° around Y so that the square lies on the wall.
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(markerScale));
        shader->setMat4("model", model);
        shader->setVec3("objectColor", markerColor);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // Right Wall: at x = 20, centered at (20, 5, 0).
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(20.0f, 5.0f, 0.0f));
        // Rotate -90° around Y so that the square lies on the wall.
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(markerScale));
        shader->setMat4("model", model);
        shader->setVec3("objectColor", markerColor);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glBindVertexArray(0);
}

Scene::Scene(int width, int height) : activeCameraIndex(0), screenWidth(width), screenHeight(height) {
    // Global camera: fixed position to view the entire scene.
    Camera* globalCamera = new Camera(GLOBAL);
    globalCamera->setPosition(glm::vec3(0.0f, 5.0f, 10.0f));
    globalCamera->setTarget(glm::vec3(0.0f, 0.0f, 0.0f));

    // Chopper camera: rotates above the scene.
    Camera* chopperCamera = new Camera(CHOPPER);
    // Initial position will be updated via its update() method.
    chopperCamera->setPosition(glm::vec3(0.0f, 10.0f, 0.0f));
    chopperCamera->setTarget(glm::vec3(0.0f, 0.0f, 0.0f));

    // First-person camera: attached to the drone.
    Camera* fpCamera = new Camera(FIRST_PERSON);
    // Its position and target will be updated every frame based on the drone.
    fpCamera->setPosition(glm::vec3(0.0f, 2.0f, 1.0f));
    fpCamera->setTarget(glm::vec3(0.0f, 2.0f, 0.0f));

    cameras.push_back(globalCamera);
    cameras.push_back(chopperCamera);
    cameras.push_back(fpCamera);
}

void Scene::update() {
    float deltaTime = 0.016f;

    // Update the drone's state.
    drone.update(deltaTime);

    // Update the chopper camera.
    for (auto cam : cameras) {
        if (cam->getType() == CHOPPER) {
            cam->update(deltaTime); // This updates its position
            // Now update its target so that it always looks at the drone.
            cam->setTarget(drone.getPosition());
        }
    }

    // Update the first-person camera.
    Camera* fpCamera = cameras[2];
    // Adjust the offset
    glm::vec3 cockpitOffset = glm::vec3(0.0f, 0.3f, -0.5f);
    float yaw = glm::radians(drone.getRotation().y);
    glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0, 1, 0));
    glm::vec3 rotatedOffset = glm::vec3(rotationMat * glm::vec4(cockpitOffset, 1.0f));

    fpCamera->setPosition(drone.getPosition() + rotatedOffset);
    // The target for the cockpit camera is set in the drone's forward direction.
    fpCamera->setTarget(drone.getPosition() + drone.getFront());
}

// Render coordinate axes at the origin.
void Scene::renderMarkers(Shader* shader) {
    static unsigned int markerVAO = 0, markerVBO = 0;
    if (markerVAO == 0) {
        float markers[] = {
                // X axis: from (0,0,0) to (1,0,0)
                0.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f,
                // Y axis: from (0,0,0) to (0,1,0)
                0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                // Z axis: from (0,0,0) to (0,0,1)
                0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f
        };

        glGenVertexArrays(1, &markerVAO);
        glGenBuffers(1, &markerVBO);
        glBindVertexArray(markerVAO);
        glBindBuffer(GL_ARRAY_BUFFER, markerVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(markers), markers, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    shader->use();
    glm::mat4 model = glm::mat4(1.0f);
    shader->setMat4("model", model);

    glBindVertexArray(markerVAO);

    // Draw X axis in red.
    shader->setVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
    glDrawArrays(GL_LINES, 0, 2);

    // Draw Y axis in green.
    shader->setVec3("objectColor", glm::vec3(0.0f, 1.0f, 0.0f));
    glDrawArrays(GL_LINES, 2, 2);

    // Draw Z axis in blue.
    shader->setVec3("objectColor", glm::vec3(0.0f, 0.0f, 1.0f));
    glDrawArrays(GL_LINES, 4, 2);

    glBindVertexArray(0);
}

void Scene::render(Shader* shader) {
    // Set up the active camera.
    Camera* cam = getActiveCamera();

    // Use the shader and set the view and projection matrices.
    shader->use();
    shader->setMat4("view", cam->getViewMatrix());
    shader->setMat4("projection", cam->getProjectionMatrix());

    // Render the ground plane.
    glm::mat4 groundModel = glm::mat4(1.0f);
    groundModel = glm::scale(groundModel, glm::vec3(40.0f, 1.0f, 40.0f));
    shader->setMat4("model", groundModel);
    shader->setVec3("objectColor", glm::vec3(0.3f, 0.8f, 0.3f));

    // Render markers to indicate 3D space.
    renderMarkers(shader);
    renderWallMarkers(shader);

    // Render the drone.
    drone.render(shader);
}

Drone* Scene::getDrone() {
    return &drone;
}

void Scene::setActiveCamera(int index) {
    if(index >= 0 && index < cameras.size()) {
        activeCameraIndex = index;
    }
}

Camera* Scene::getActiveCamera() {
    return cameras[activeCameraIndex];
}

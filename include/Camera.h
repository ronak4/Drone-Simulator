#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraType {
    GLOBAL,
    CHOPPER,
    FIRST_PERSON
};

class Camera {
public:
    Camera(CameraType type);

    // Update camera properties
    void update(float deltaTime);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    void setPosition(const glm::vec3 &position);
    void setTarget(const glm::vec3 &target);
    void setUp(const glm::vec3 &up);

    CameraType getType() const;

    // For chopper camera rotation control.
    void setAngle(float angle);

private:
    CameraType type;
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;

    // Projection parameters.
    float fov;
    float aspectRatio;
    float nearClip;
    float farClip;

    // For chopper camera rotation.
    float angle;
};

#endif // CAMERA_H

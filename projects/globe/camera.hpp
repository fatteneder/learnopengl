#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

enum CameraMovement {
  LEFT,
  RIGHT,
  UP,
  DOWN,
};

class Camera {
  public:
    glm::vec3 center;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    float radius;
    float angularSpeed;
    float zoom;

    Camera(glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f), float radius = 1.0f)
    {
      this->center = center;
      this->position = center + glm::vec3(0.0f, 0.0f, radius);
      this->front = glm::normalize(center - position);
      this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
      this->radius = radius;
      this->angularSpeed = 1.0f;
      this->zoom = 45.0f;
      updateCameraVectors();
    }

    glm::mat4 getViewMatrix()
    {
      return glm::lookAt(position, position + front, up);
    }

    void processKeyboard(CameraMovement dir, float dt)
    {
      auto rot = glm::mat4(1.0f);
      float phi = angularSpeed*dt;
      switch (dir) {
        // move along longitude
        case UP:    rot = glm::rotate(rot, -phi, glm::vec3(1.0f, 0.0f, 0.0f)); break;
        case DOWN:  rot = glm::rotate(rot, phi,  glm::vec3(1.0f, 0.0f, 0.0f)); break;
        // move along latitude
        case RIGHT: rot = glm::rotate(rot, phi,  glm::vec3(0.0f, 1.0f, 0.0f)); break;
        case LEFT:  rot = glm::rotate(rot, -phi, glm::vec3(0.0f, 1.0f, 0.0f)); break;
      }
      position = center + glm::vec3(rot * glm::vec4(position - center, 1.0f));
      updateCameraVectors();
    }

    void processMouseScroll(float yoffset)
    {
      zoom -= (float)yoffset;
      zoom = std::clamp(zoom, 1.0f, 45.0f);
    }

    void updateCameraVectors()
    {
      front = glm::normalize(center - position);
      right = glm::normalize(glm::cross(front, worldUp));
      up = glm::normalize(glm::cross(right, front));
    }

};

#endif

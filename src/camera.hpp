#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

enum CameraMovement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
};

class Camera {
  public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    float yaw;
    float pitch;
    float moveSpeed;
    float mouseSens;
    float zoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f,
           float pitch = 0.0)
    {
      this->position = position;
      this->front = glm::vec3(0.0f, 0.0f, -1.0f);
      this->worldUp = up;
      this->yaw = yaw;
      this->pitch = pitch;
      this->moveSpeed = 5.0f;
      this->mouseSens = 0.05f;
      this->zoom = 45.0f;
      updateCameraVectors();
    }

    glm::mat4 getViewMatrix()
    {
      return glm::lookAt(position, position + front, up);
    }

    void processKeyboard(CameraMovement dir, float dt)
    {
      const float dist = moveSpeed * dt;
      switch(dir) {
        case FORWARD: position += front * dist; break;
        case BACKWARD: position -= front * dist; break;
        case RIGHT: position += right * dist; break;
        case LEFT: position -= right * dist; break;
        default: assert(false);
      }
    }

    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
    {
      xoffset *= mouseSens;
      yoffset *= mouseSens;
      yaw   += xoffset;
      pitch += yoffset;
      if (constrainPitch) pitch = std::clamp(pitch, -89.0f, 89.0f);
      updateCameraVectors();
    }

    void processMouseScroll(float yoffset)
    {
      zoom -= (float)yoffset;
      zoom = std::clamp(zoom, 1.0f, 45.0f);
    }

    void updateCameraVectors()
    {
      glm::vec3 newFront;
      newFront.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
      newFront.y = std::sin(glm::radians(pitch));
      newFront.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
      front = glm::normalize(newFront);
      right = glm::normalize(glm::cross(front, worldUp));
      up = glm::normalize(glm::cross(right, front));
    }

};

#endif

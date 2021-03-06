#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "Settings.h"
#include "imgui/imgui.h"
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MouseSensitivity;
    float Zoom;

    bool isOrthographic = false;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 GetProjection()
    {
        auto& settings = Settings::GetInstance();

        if (isOrthographic) {
            const float aspect = (float)settings.screenHeight / (float)settings.screenWidth;
            return glm::ortho(-Zoom, Zoom, -Zoom * aspect, Zoom * aspect, nearPlane, farPlane);
        }
        else {
            const float aspect = (float)settings.screenWidth / (float)settings.screenHeight;
            return glm::perspective(glm::radians(Zoom), aspect, nearPlane, farPlane);
        }
    }

    void InversePitch()
    {
        Pitch = -Pitch;
        updateCameraVectors();
    }
    void InverseYaw()
    {
        Yaw = -Yaw;
        updateCameraVectors();
    }
    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void Move(Camera_Movement direction, float velocity)
    {
        switch (direction)
        {
        case Camera_Movement::FORWARD:
            Position += Front * velocity;
            break;
        case Camera_Movement::BACKWARD:
            Position -= Front * velocity;
            break;
        case Camera_Movement::LEFT:
            Position -= Right * velocity;
            break;
        case Camera_Movement::RIGHT:
            Position += Right * velocity;
            break;
        case Camera_Movement::UP:
            Position += Up * velocity;
            break;
        case Camera_Movement::DOWN:
            Position -= Up * velocity;
            break;
        default:
            break;
        } 
        updateCameraVectors();
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void FollowMouse(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

    void UpdateUpAndRight(glm::vec3 front)
    {
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }

    void OnGUI()
    {
        if (ImGui::TreeNode("Camera"))
        {
            ImGui::Checkbox("Is Orthographic", &isOrthographic);

            ImGui::DragFloat("Near", &nearPlane, 0.1f, 0.1f, 10000000.0f);
            ImGui::DragFloat("Far", &farPlane, 0.1f, 0.1f, 10000000.0f);
            if (nearPlane > farPlane)
                nearPlane = farPlane;

            ImGui::DragFloat("Mouse Sensitivity", &MouseSensitivity, 0.1f, 0.0f, 1000000.0f);
            ImGui::DragFloat("Zoom", &Zoom, 0.1f, 1.0f, 120.0f);
            ImGui::TreePop();
        }
    }
private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        UpdateUpAndRight(front);
    }
};
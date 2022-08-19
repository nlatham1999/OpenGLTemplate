#ifndef CAMERA2_H
#define CAMERA2_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
\
#include <iostream>
#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement2 {
    FORWARD2,
    BACKWARD2,
    LEFT2,
    RIGHT2
};

// Default camera values
const float YAW2 = -90.0f;
const float PITCH2 = 0.0f;
const float SPEED2 = .001f;
const float SENSITIVITY2 = 0.1f;
const float ZOOM2 = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera2
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Target;
    glm::vec3 Up;

    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    bool switchDirection = true;

    // constructor with vectors
    Camera2(glm::vec3 position = glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW2, float pitch = PITCH2) 
        : Target(glm::vec3(0.0f, 0.0f, 0.0f)), 
        MovementSpeed(SPEED2), 
        MouseSensitivity(SENSITIVITY2), 
        Zoom(ZOOM2)
    {

        Position = position;
        Up = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera2(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
        : Target(glm::vec3(0.0f, 0.0f, -1.0f)), 
        MovementSpeed(SPEED2), 
        MouseSensitivity(SENSITIVITY2), 
        Zoom(ZOOM2)
    {
        Position = glm::vec3(posX, posY, posZ);
        Up = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(
            Position,
            Target,
            Up);
        //return glm::lookAt(glm::vec3(0.0f, 0.0f, TargetZ), Target, Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement2 direction, float deltaTime)
    {
        //float velocity = MovementSpeed * deltaTime;
        //if (direction == FORWARD2)
        //    Position += Front * velocity;
        //if (direction == BACKWARD2)
        //    Position -= Front * velocity;
        //if (direction == LEFT2)
        //    Position -= Right * velocity;
        //if (direction == RIGHT2)
        //    Position += Right * velocity;
    }

    void MoveForward(float deltaTime) {
        //float velocity = MovementSpeed * deltaTime;
        Position.z -= MovementSpeed;
        //TargetZ += MovementSpeed;
    }

    void MoveBackward(float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        //Position.x += velocity;
        Position.z += MovementSpeed;
    }

    void StrafeLeft(float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        //Position -= Right * velocity;

        //Position.x -= MovementSpeed;
        //Target.x -= MovementSpeed;

        Position.x -= cos(Position.x) * MovementSpeed;
        Target.x -= cos(Target.x) * MovementSpeed;

        Position.z += sin(Position.x) * MovementSpeed;
        Target.z += sin(Target.x) * MovementSpeed;
    }

    void StrafeRight(float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
       //Position += Right * velocity;

        Position.x += MovementSpeed;
        Target.x += MovementSpeed;
    }

    void RotateUp(float deltaTime) {

        Position.y = cos(MovementSpeed) * Position.y - sin(MovementSpeed) * Position.z;
        Position.z = sin(MovementSpeed) * Position.y + cos(MovementSpeed) * Position.z;
        
        std::cout << Position.y << " " << Position.z << std::endl;

        if (Position.y < -5.96 || Position.y > 5.96) {
            if (switchDirection) {
                Up.y *= -1;
                switchDirection = false;
            }
        }
        else {
            switchDirection = true;
        }
    }

    void RotateDown(float deltaTime) {
        Position.y = cos(-MovementSpeed) * Position.y - sin(-MovementSpeed) * Position.z;
        Position.z = sin(-MovementSpeed) * Position.y + cos(-MovementSpeed) * Position.z;
    }

    void RotateLeft(float deltaTime) {
        Position.x = cos(MovementSpeed) * Position.x + sin(MovementSpeed) * Position.z;
        Position.z = -sin(MovementSpeed) * Position.x + cos(MovementSpeed) * Position.z;
    }

    void RotateRight(float deltaTime) {
        Position.x = cos(-MovementSpeed) * Position.x + sin(-MovementSpeed) * Position.z;
        Position.z = -sin(-MovementSpeed) * Position.x + cos(-MovementSpeed) * Position.z;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
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

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        //Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        //Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        //Up = glm::normalize(glm::cross(Right, Front));
    }
};
#endif
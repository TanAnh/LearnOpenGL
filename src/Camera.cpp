#include "Camera.h"

void Camera::updateCameraVectors()
// calculates the front vector from the Camera's (updated) Euler Angles
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
// constructor with vectors
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
// constructor with scalar values
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
// returns the view matrix calculated using Euler Angles and the LookAt Matrix
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += glm::vec3(Front.x, 0.0f, Front.z) * velocity;   // Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= glm::vec3(Front.x, 0.0f, Front.z) * velocity;   // Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
    if (direction == UP)
        Position += glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
    if (direction == DOWN)
        Position -= glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
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

void Camera::ProcessMouseScroll(float yoffset)
// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

glm::vec3 Camera::getPosition()
{
    return Position;
}

glm::vec3 Camera::getFront()
{
    return Front;
}

glm::vec3 Camera::getUp()
{
    return Up;
}

glm::vec3 Camera::getRight()
{
    return Right;
}

glm::vec3 Camera::getWorldUp()
{
    return WorldUp;
}

float Camera::getYaw()
{
    return Yaw;
}

float Camera::getPitch()
{
    return Pitch;
}

float Camera::getMovementSpeed()
{
    return MovementSpeed;
}

float Camera::getMouseSensitivity()
{
    return MouseSensitivity;
}

float Camera::getZoom()
{
    return Zoom;
}

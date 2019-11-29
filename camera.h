#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include "glm-master/glm/glm.hpp"
#include "glm-master/glm/gtc/matrix_transform.hpp"

#include <vector>

//Define as posibilidades de movimentação da camera
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

//Valores padroes
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  10.0f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


//Classe camera para calculcular matrizes angulos e relacionados a camera e processar os inputs
class Camera
{
public:
    //Atributos
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    //Angulo
    float Yaw;
    float Pitch;
    //Caracteristicas da camera
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    //Construtor com vec3 do glm
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    //Construtor com valores
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    //Retorna a matriz do view baseada na posição da camera
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    //Processa os inputs do teclado
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    //Processa os inputs do mouse
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        //Não deixa o pitch sair da borda(tela gira)
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        //Atualiza os vectors da camera
        updateCameraVectors();
    }

    //Processa input do scroll do mouse
    void ProcessMouseScroll(float yoffset)
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }

private:
    //Calcula os vectors da camera
    void updateCameraVectors()
    {
        //Calcula o front Vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        //Recalcula o right e o up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  //Normaliza dos vector
        Up    = glm::normalize(glm::cross(Right, Front));    //O tamanho fica mais proximo de 0 quando se olha para baixo, oq resulta em movimentos mais lentos
    }
};
#endif


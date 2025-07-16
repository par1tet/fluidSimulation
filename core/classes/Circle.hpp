#pragma once

#include<glm/glm.hpp>
#include<glad/glad.h>
#include<utils/constants.hpp>

class Circle{
public:
    glm::vec3 position, velocity, acceleration;
    float radius;
    
    Circle(glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration, float radius);

    void drawCircle();
    void update(float dt);
};
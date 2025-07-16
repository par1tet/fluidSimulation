#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glad/glad.h>
#include<utils/constants.hpp>
#include<vector>

class Circle{
public:
    glm::vec3 position, velocity, acceleration, color;
    float radius;
    
    Circle(glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration, float radius);

    void drawCircle(GLuint shaderProgram);
    void update(float dt);
    void physical(std::vector<Circle*> otherCircles);
};
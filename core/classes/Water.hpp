#pragma once

#include<vector>
#include<classes/Circle.hpp>

class Water{
public:
    std::vector<Circle*> particles;

    Water(int countParticles, float gap);
    void update(float dt);
    void draw(GLuint shaderProgram);
};
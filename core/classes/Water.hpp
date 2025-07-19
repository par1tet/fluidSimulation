#pragma once

#include<vector>
#include<classes/Circle.hpp>

class Water{
public:
    std::vector<Circle*> particles;
    SPHConstans config;

    Water(int countParticles, float gap, SPHConstans config);
    void update(float dt);
    void draw(GLuint shaderProgram);
};
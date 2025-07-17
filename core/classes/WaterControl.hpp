#pragma once

#include<classes/Water.hpp>
#include<iostream>
#include<GLFW/glfw3.h>

class WaterControl{
public:
    bool isLeftMousePressed = false, isRightMousePressed = false;
    Water* water;
    GLFWwindow* window;
    glm::vec2 mousePos{0.0f};

    WaterControl(GLFWwindow* window, Water* water);
    void update();
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void attractParticles();
    void pushAwayParticles();
};
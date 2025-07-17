#include<classes/WaterControl.hpp>

WaterControl::WaterControl(GLFWwindow* window, Water* water){
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    glfwSetWindowUserPointer(window, this);

    this->water = water;
    this->window = window;
}

void WaterControl::update() {
    if (this->isLeftMousePressed) {
        this->attractParticles();
    }

    if (this->isRightMousePressed) {
        this->pushAwayParticles();
    }
}    

void WaterControl::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    WaterControl* handler = static_cast<WaterControl*>(glfwGetWindowUserPointer(window));
    if (handler) {
        handler->mousePos.x = static_cast<float>(xpos);
        handler->mousePos.y = static_cast<float>(ypos);
    }
}

void WaterControl::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    WaterControl* handler = static_cast<WaterControl*>(glfwGetWindowUserPointer(window));
    if (!handler) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        handler->isLeftMousePressed = (action == GLFW_PRESS);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        handler->isRightMousePressed = (action == GLFW_PRESS);
    }
}

void WaterControl::attractParticles(){
    glm::vec3 thisMouse3v = {this->mousePos.x - WIDTH/2, HEIGHT/2 - this->mousePos.y, 0.f};

    std::cout << thisMouse3v.y << std::endl;

    for(int i = 0;i != this->water->particles.size();i++){
        float distance = glm::distance(this->water->particles[i]->position, thisMouse3v);
        glm::vec3 direct = this->water->particles[i]->position - thisMouse3v;

        if(distance <= 120){
            std::cout << "pines" << std::endl;
            this->water->particles[i]->velocity -= glm::normalize(direct) * 120.f;
        }
    }
}

void WaterControl::pushAwayParticles(){
    glm::vec3 thisMouse3v = {this->mousePos.x - WIDTH/2, HEIGHT/2 - this->mousePos.y, 0.f};

    std::cout << thisMouse3v.y << std::endl;

    for(int i = 0;i != this->water->particles.size();i++){
        float distance = glm::distance(this->water->particles[i]->position, thisMouse3v);
        glm::vec3 direct = this->water->particles[i]->position - thisMouse3v;

        if(distance <= 120){
            std::cout << "pines" << std::endl;
            this->water->particles[i]->velocity += glm::normalize(direct) * 120.f;
        }
    }
}
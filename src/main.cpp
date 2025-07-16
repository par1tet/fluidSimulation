#include<iostream>
#include<utils/functions.hpp>
#include<utils/shaderInit.hpp>
#include<classes/Circle.hpp>

int main(){
    GLFWwindow* window = initGlfwWindowAndGlad();
    if(window == nullptr){
        return -1;
    }

    GLuint shaderProgram = createShaderProgram(
      	std::string(std::string(CMAKE_SOURCE) + "shaders/vertex_shader.glsl").c_str(),
      	std::string(std::string(CMAKE_SOURCE) + "shaders/fragment_shader.glsl").c_str()
    );   
    
    Circle* circle = new Circle(glm::vec3{0.f}, glm::vec3{0.f}, glm::vec3{0.f, -9.8f, 0.f}, 10.f);

    float dt = 1/60.f;

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        circle->drawCircle();

        circle->update(dt);
        
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
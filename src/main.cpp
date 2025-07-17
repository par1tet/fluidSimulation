#include<iostream>
#include<utils/functions.hpp>
#include<utils/shaderInit.hpp>
#include<classes/Water.hpp>
#include<classes/WaterControl.hpp>

int main(){
    GLFWwindow* window = initGlfwWindowAndGlad();
    if(window == nullptr){
        return -1;
    }

    GLuint shaderProgram = createShaderProgram(
      	std::string(std::string(CMAKE_SOURCE) + "shaders/vertex_shader.glsl").c_str(),
      	std::string(std::string(CMAKE_SOURCE) + "shaders/fragment_shader.glsl").c_str()
    );   
    
    Water* water = new Water(20, 15);
    WaterControl* waterControl = new WaterControl(window, water);

    float dt = 1/60.f;
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        water->draw(shaderProgram);
        water->update(dt);
        waterControl->update();
        
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
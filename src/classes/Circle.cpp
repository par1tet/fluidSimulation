#include<classes/Circle.hpp>
#include<vector>
#include<iostream>

std::vector<float> generateCircleVertices(float centerX, float centerY, float radius, int numSegments) {
    std::vector<float> vertices = {};
    float angleIncrement = 2.0f * M_PI / numSegments;

    for (int i = 0; i < numSegments; ++i) {
        float angle = i * angleIncrement;
        float x = centerX + radius * cos(angle) / WIDTH;
        float y = centerY + radius * sin(angle) / HEIGHT;
        vertices.push_back(x);
        vertices.push_back(y);
    }

    return vertices;
}

Circle::Circle(glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration, float radius){
    this->position = position;
    this->velocity = velocity;
    this->acceleration = acceleration;
    this->radius = radius;
}

void Circle::drawCircle(){
    std::vector<float> vertices = generateCircleVertices(this->position.x,this->position.y,this->radius, 30);
    
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);
    glBindVertexArray(0);

}

void Circle::update(float dt){
    this->velocity += this->acceleration * dt;
    this->position += this->velocity * dt;

    if(this->position.y < (-1.f + this->radius/HEIGHT)){
        this->position.y = (-1.f + this->radius/HEIGHT);
        this->velocity.y *= 0;
    }
    if(this->position.x < -WIDTH/2){
        this->position.x = -WIDTH/2;
        this->velocity.x *= 0;
    }
    if(this->position.x > WIDTH/2){
        this->position.x = WIDTH/2;
        this->velocity.x *= 0;
    }

    //std::cout << position.y << std::endl;
}
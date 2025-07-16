#include<classes/Circle.hpp>
#include<iostream>

std::vector<float> generateCircleVertices(float centerX, float centerY, float radius, int numSegments) {
    std::vector<float> vertices = {};
    float angleIncrement = 2.0f * M_PI / numSegments;

    for (int i = 0; i < numSegments; ++i) {
        float angle = i * angleIncrement;
        float x = (centerX + radius * cos(angle)) / (WIDTH/2);
        float y = (centerY + radius * sin(angle)) / (HEIGHT/2);
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
    this->color = glm::vec3{1.f};
}

void Circle::drawCircle(GLuint shaderProgram){
    std::vector<float> vertices = generateCircleVertices(this->position.x,this->position.y,this->radius, 30);
    
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    GLuint colorLocation = glGetUniformLocation(shaderProgram, "colorCircle");
    glUniform3fv(colorLocation, 1, glm::value_ptr(this->color));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);
    glBindVertexArray(0);

}

void Circle::update(float dt){
    this->velocity += this->acceleration * dt;
    this->position += this->velocity * dt;
}

void Circle::physical(std::vector<Circle*> otherCircles){
    if(this->position.y < (-HEIGHT/2 + this->radius)){
        this->position.y = (-HEIGHT/2 + this->radius);
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

    for(int i = 0;i != otherCircles.size();i++){
        float distantce = glm::distance(this->position, otherCircles[i]->position);

        if(distantce == 0){continue;}

        glm::vec3 direct = this->position - otherCircles[i]->position;

        if(distantce < (this->radius + otherCircles[i]->radius)){
            this->position -= glm::normalize(direct) * (distantce - (this->radius + otherCircles[i]->radius));
            this->velocity *= (.0f) * -1;
        }

        if(distantce <= 40){
            this->velocity += glm::normalize(direct)*(10.f);
        }
    }

    this->color.y = 1.f - glm::length(this->velocity)/150.f;
    this->color.z = 1.f - glm::length(this->velocity)/150.f;
}
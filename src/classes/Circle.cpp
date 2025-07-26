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

float kernelFunction(float distance, float smoothRadius){
    float q = distance / (smoothRadius);
    float sigma = 10.f/(7.f*M_PI*smoothRadius*smoothRadius);

    if(0 <= q && q < 1){
        return sigma * (1 - (3.f/2.f)*(q*q) + (3.f/4.f)*(q*q*q));
    }else if(1 <= q && q < 2){
        return sigma * (((2-q)*(2-q)*(2-q))/4.f);
    }

    return 0.f;
}

glm::vec3 gradientKernelFunction(glm::vec3 d, float smoothRadius) {
    float r = glm::length(d);
    if (r < 1e-5f) return glm::vec3(0.f);
    
    float q = r / smoothRadius;
    float sigma = 10.f / (7.f * M_PI * smoothRadius * smoothRadius);
    
    float dW_dq;
    if (0 <= q && q < 1) {
        dW_dq = sigma * (-3.f * q + (9.f/4.f) * q * q);
    } else if (1 <= q && q < 2) {
        dW_dq = sigma * (-3.f/4.f) * (2.f - q) * (2.f - q);
    } else {
        return glm::vec3(0.f);
    }
    
    return (dW_dq / smoothRadius) * (d / r);
}

std::vector<float> calcDensityField(std::vector<Circle*> circles, SPHConstans config) {
    std::vector<float> densities(circles.size(), 0.f);
    
    for(int i = 0; i < circles.size(); i++) {
        for(int j = 0; j < circles.size(); j++) {
            if(i == j) continue;
            
            float dist = glm::distance(circles[i]->position, circles[j]->position);
            if(dist < 2.f * config.smoothRadius) {
                densities[i] += config.massParticle * kernelFunction(dist, config.smoothRadius);
            }
        }
        densities[i] = fmax(densities[i], config.density * 0.1f);

        std::cout << densities[i] << std::endl;
    }

    
    return densities;
}

std::vector<float> calcPreassureField(std::vector<float> densityField, SPHConstans config){
    std::vector<float> preassureField;

    for(int i = 0;i != densityField.size();i++){
        preassureField.push_back(config.rigidity*(densityField[i]-config.density)); 
    }
    
    return preassureField;
}

glm::vec3 preassureForce(std::vector<Circle*> otherCircles, int currentI, std::vector<float> densityField, std::vector<float> preassureField,SPHConstans config){
    glm::vec3 sum = glm::vec3{.0f};
    glm::vec3 currentPosition = otherCircles[currentI]->position;

    for(int j = 0;j != otherCircles.size();j++){
        if(j == currentI) continue;
        glm::vec3 d = currentPosition - otherCircles[j]->position;
        float preassurePart = (
            (preassureField[currentI]/(densityField[currentI]*densityField[currentI]))-
            (preassureField[j]/(densityField[j]*densityField[j]))
        );

        sum += config.massParticle * preassurePart * gradientKernelFunction(d, config.smoothRadius);
    }

    std::cout << "x: " << sum.x << "y: " << sum.y << "z: " << sum.z << std::endl;;

    return -sum;
}

void Circle::physical(std::vector<Circle*> otherCircles, SPHConstans config){
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

    this->acceleration = glm::vec3{.0f, -980.f, .0f};

    for(int i = 0;i != otherCircles.size();i++){
        float distance = glm::distance(this->position, otherCircles[i]->position);
        glm::vec3 direct = this->position - otherCircles[i]->position;

        if(distance == 0){continue;}
        
        if(distance < (this->radius + otherCircles[i]->radius)){
            this->position -= glm::normalize(direct) * (distance - (this->radius + otherCircles[i]->radius));
            this->velocity *= (0.0f) * -1;
        }

        // SPH

        std::vector<float> densityField = calcDensityField(otherCircles, config);
        std::vector<float> preassureField = calcPreassureField(densityField, config);

        glm::vec3 preassureForceParticle = preassureForce(otherCircles, i, densityField, preassureField, config);
        
        this->acceleration += preassureForceParticle / config.massParticle;
    }

    this->color.y = 1.f - glm::length(this->velocity)/150.f;
    this->color.z = 1.f - glm::length(this->velocity)/150.f;
}
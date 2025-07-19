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

float calculateMagnitude(float distance, float radius){
    float a = 1/3;

    float absDist = sqrt(distance*distance);
    float max = glm::max(0.f, radius*radius - (absDist*absDist*a*a));

    return max*max*max/a;
}

float kernelFunction(float distance, float smoothRadius){
    float q = distance / (smoothRadius * 100);
    float sigma = 10/(7*M_PI);

    if(0 <= q < 1){
        return sigma * (1 - (3/2)*(q*q) + (3/4)*(q*q*q));
    }else if(1 <= q < 2){
        return sigma * (((2-q)*(2-q)*(2-q))/4);
    }

    return 0;
}

float gradientKernelFunction(float distance, float smoothRadius){
    float q = distance/(smoothRadius*100);
    float sigma = 10/(7*M_PI*(smoothRadius*smoothRadius*smoothRadius*smoothRadius*100000000));

    if(0 <= q < 1){
        return sigma * distance * (-3*q + (9/4)*(q*q));
    }else if(1 <= q < 2){
        return sigma * distance * ((-3/4)*((2-q)*(2-q)));
    }

    return 0;
}

std::vector<float> calcDensityField(std::vector<Circle*> otherCircles, SPHConstans config){
    std::vector<float> densityField;
    float jSum = 0;

    for(int i = 0;i != otherCircles.size();i++){
        jSum = 0;

        glm::vec3 currentPosition = otherCircles[i]->position;

        for(int j = 0;j != otherCircles.size();j++){
            if(i == j) continue;

            float distance = glm::distance(currentPosition, otherCircles[j]->position);

            jSum += config.massParticle * kernelFunction(distance, config.smoothRadius);
        }

        densityField.push_back(jSum);
    }

    return densityField;
}

std::vector<float> calcPreassureField(std::vector<float> densityField, SPHConstans config){
    std::vector<float> preassureField;

    for(int i = 0;i != densityField.size();i++){
        preassureField.push_back(config.rigidity*(densityField[i]-config.rigidity)); 
    }
    
    return preassureField;
}

float preassureForce(std::vector<Circle*> otherCircles, int currentI, std::vector<float> densityField, std::vector<float> preassureField,SPHConstans config){
    float sum = 0;
    glm::vec3 currentPosition = otherCircles[currentI]->position;

    for(int j = 0;j != otherCircles.size(), j != currentI;j++){
        float distance = glm::distance(otherCircles[j]->position, currentPosition);
        float preassurePart = (
            (preassureField[currentI]/(densityField[currentI]*densityField[currentI]))-
            (preassureField[j]/(densityField[j]*densityField[j]))
        );

        sum += config.massParticle * preassurePart * gradientKernelFunction(distance, config.smoothRadius);
    }

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

        if(distance == 0){continue;}

        // SPH

        std::vector<float> densityField = calcDensityField(otherCircles, config);
        std::vector<float> preassureField = calcPreassureField(densityField, config);
        
        std::cout << i << std::endl;
    }

    std::cout << "pines 1" << std::endl;

    this->color.y = 1.f - glm::length(this->velocity)/150.f;
    this->color.z = 1.f - glm::length(this->velocity)/150.f;
}
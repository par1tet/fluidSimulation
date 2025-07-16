#include<classes/Water.hpp>

Water::Water(int countParticlesRoot, float gap){
    for(int i = 0;i != countParticlesRoot;i++){
        for(int j = 0;j != countParticlesRoot;j++){
            Circle* newCircle = new Circle(glm::vec3{((-ceil(countParticlesRoot/2) + (i*(gap+1)))),(-ceil(countParticlesRoot/2) + (j*(gap+1))),0.f}, glm::vec3(0.f), glm::vec3{0.f, -980.f,0.f}, 5);

            this->particles.push_back(newCircle);
        }
    }
}

void Water::update(float dt){
    for(int i = 0;i != this->particles.size();i++){
        this->particles[i]->update(dt);
    }
    for(int i = 0;i != this->particles.size();i++){
        this->particles[i]->physical(this->particles);
    }
}

void Water::draw(GLuint shaderProgram){
    for(int i = 0;i != this->particles.size();i++){
        this->particles[i]->drawCircle(shaderProgram);
    }
}
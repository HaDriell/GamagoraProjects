#include <iostream>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

const glm::vec3 GRAVITY{0, 0, -9.8};
const glm::vec3 WIND(-10, 0, 0);

struct Body
{
    float mass;
    float friction;
    glm::vec3 position;
    glm::vec3 speed;
};

void physic_timestep(Body& body, float timestep);
glm::vec3 get_acceleration(Body& body);

int main()
{
    Body small;
    small.mass = 85.0f;
    small.friction = 0.33f;
    small.position = {0.0f, 0.0f, 0.0f};
    small.speed = {144.0f, 0.0f, 0.0f};

    Body big;
    big.mass = 85.0f;
    big.friction = 0.17;
    big.position = {0.0f, 0.0f, 0.0f};
    big.speed = {144.0f, 0.0f, 0.0f};


    std::ofstream out;
    out.open("output.csv");

    out << "Timestep,SmallX,SmallZ,BigX,BigZ" << std::endl;

    for (int timestep = 0; timestep < 100; timestep++)
    {
        physic_timestep(small, 1.0f);
        physic_timestep(big, 1.0f);

        out << std::scientific 
        << timestep << ','
        << small.position.x << ',' << small.position.z << ',' 
        << big.position.x << ',' << big.position.z
        << std::endl;
    }

    out.close();
}

glm::vec3 get_acceleration(Body& body)
{
    //Friction
    glm::vec3 friction = -body.speed * glm::length(body.speed) * body.friction; 

    //And Gravity
    return GRAVITY + (friction + WIND) / body.mass;
}

void physic_timestep(Body& body, float timestep)
{
    //Acceleration
    glm::vec3 acceleration = get_acceleration(body);

    //Update Body
    body.speed += acceleration * timestep;
    body.position += body.speed * timestep;
}
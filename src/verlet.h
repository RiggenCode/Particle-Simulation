#ifndef __VERLET_H__
#define __VERLET_H__

#define GRAVITY -10.0f

#include <glm/glm.hpp>

struct verletObjects
{
    glm::vec3 position;
    glm::vec3 oldPosition;
    glm::vec3 acceleration;
    float radius = 1.0f;
};

class Verlet {
private:
    glm::vec3 position;
    glm::vec3 oldPosition;
    glm::vec3 acceleration;
    float radius = 1.0f;

public:
    void applyForce(verletObjects* objects, int size);
    void handleCollisions(verletObjects* a, verletObjects* b);
    void applyCollisions(verletObjects* objects, int size);
    void updatePositions(verletObjects* objects, int size, float dt);

    void clearGrid();
    void fillGrid(verletObjects* objects, int size);
    void applyGridCollisions();

    void addForce(verletObjects* objects, int size, glm::vec3 force);
};


#endif
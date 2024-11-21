#include "verlet.h"

void Verlet::applyForce(verletObjects* objects, int size) {
    for (int i = 0; i < size; i++) {
        objects[i].acceleration[1] += GRAVITY;
    }
}

void Verlet::handleCollisions(verletObjects* a, verletObjects* b) {
    float distance = glm::distance(a->position, b->position);
    if (distance < a->radius + b->radius) {
        glm::vec3 normal = glm::normalize(a->position - b->position);
        glm::vec3 relativeVelocity = a->position - a->oldPosition - (b->position - b->oldPosition);
        float velocityAlongNormal = glm::dot(relativeVelocity, normal);
        if (velocityAlongNormal < 0) {
            glm::vec3 impulse = (-(1.0f + 0.8f) * velocityAlongNormal) * normal;
            a->position += impulse;
        }
    }
}

void Verlet::applyCollisions(verletObjects* objects, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (i != j) {
                handleCollisions(&objects[i], &objects[j]);
            }
        }
    }
}
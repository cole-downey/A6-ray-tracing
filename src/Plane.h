#ifndef _PLANE_H
#define _PLANE_H

#include "Object.h"

class Plane : public Object {
protected:
    glm::vec3 nor;
public:
    Plane(glm::vec3 _pos, glm::vec3 _nor, Material _mat) : Object(_pos, glm::vec3(1.0f), glm::vec3(0.0f), _mat), nor(_nor) {};
    ~Plane() {};

    void intersectTest(const glm::vec3& p, const glm::vec3& v, std::vector<intersect>& intersects);
};

#endif
#ifndef _ELLIPSOID_H_
#define _ELLIPSOID_H_

#include "Object.h"

class Ellipsoid : public Object {
protected:
    glm::mat4 ellipsoid_E;
    void calcE();
public:
    Ellipsoid(glm::vec3 _pos, glm::vec3 _scale, glm::vec3 _rot, Material _mat) : Object(_pos, _scale, _rot, _mat) { calcE(); };
    ~Ellipsoid() {};

    void intersectTest(const glm::vec3& p, const glm::vec3& v, std::vector<intersect>& intersects);
};

#endif
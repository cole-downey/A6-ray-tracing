#ifndef _MESH_H_
#define _MESH_H_

#include "Object.h"
#include "Shape.h"
#include "Ellipsoid.h"

class Mesh : public Object, public Shape {
protected:
    glm::mat4 mesh_E;
    void calcE();
    Ellipsoid* bounds;
    int triangleIntersect(int i, glm::vec3& p, glm::vec3& v, std::vector<intersect>& intersects, glm::vec3& hit, glm::vec3& hitNor);

public:
    Mesh(glm::vec3 _pos, glm::vec3 _scale, glm::vec3 _rot, Material _mat) : Object(_pos, _scale, _rot, _mat), Shape(), bounds(nullptr) { calcE(); };
    ~Mesh() { delete bounds; };
    void loadMesh(const std::string& meshName);

    void calcBounds();
    void intersectTest(const glm::vec3& p, const glm::vec3& v, std::vector<intersect>& intersects);
};

#endif
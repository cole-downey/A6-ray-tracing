#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <random>
#include <cmath>
#include <algorithm>

#include "MatrixStack.h"
#include "Material.h"

struct intersect {
    void* obj;
    float t;
    glm::vec3 pos;
    glm::vec3 nor;
    intersect(void* _obj, float _t, glm::vec3 _pos, glm::vec3 _nor) :
        obj(_obj), t(_t), pos(_pos), nor(_nor) {
    };
};


class Object {
private:
    static int globalCount;
    int id; // for easy comparisions between objects
protected:
    glm::vec3 pos;
    glm::vec3 scale;
    glm::vec3 rot;
public:
    enum {
        BLINN_PHONG = 0,
        REFLECTIVE,
        BLENDED,
        TRANS,
    };
    int shader;

    Material mat;
    Object(glm::vec3 _pos, glm::vec3 _scale, glm::vec3 _rot, Material _mat);
    ~Object();

    int getId() { return id; };
    virtual void intersectTest(const glm::vec3& p, const glm::vec3& v, std::vector<intersect>& intersects) = 0;


};

#endif
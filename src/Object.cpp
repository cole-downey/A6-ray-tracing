#include "Object.h"

using namespace std;
using namespace glm;

int Object::globalCount = 0;

Object::Object(glm::vec3 _pos, glm::vec3 _scale, glm::vec3 _rot, Material _mat) :
    pos(_pos), scale(_scale),
    rot(_rot), mat(_mat),
    id(globalCount), shader(BLINN_PHONG) {
    globalCount++;
}

Object::~Object() {
}

#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <random>
#include <cmath>
#include <algorithm>


class Light {
public:
    glm::vec3 pos;
    glm::vec3 color;
    Light(glm::vec3 _pos, glm::vec3 _color) : pos(_pos), color(_color) {};
};



#endif
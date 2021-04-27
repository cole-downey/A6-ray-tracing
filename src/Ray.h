#pragma  once
#ifndef _RAY_H_
#define _RAY_H_

#include <glm/fwd.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <vector>

#include "Object.h"
#include "Light.h"

class Ray {
private:
    static float self_t; // for self intersection tests
public:
    glm::vec3 p;
    glm::vec3 v;
    int pixX; // pixel coord
    int pixY;
    std::shared_ptr<intersect> self;
    static glm::vec3 camPos;
    static std::vector<std::shared_ptr<Light>> lights;
    static std::vector<std::shared_ptr<Object>> objects;

    Ray(glm::vec3 _p, glm::vec3 _v);
    Ray(glm::vec3 _p, glm::vec3 _v, std::shared_ptr<intersect> _self);
    Ray(glm::vec3 _p, glm::vec3 _v, int _pixX, int _pixY);
    ~Ray() {};

    std::shared_ptr<intersect> intersectTest(std::shared_ptr<intersect> _self = nullptr, bool ignoreTrans = false);
    glm::vec3 trace();
    glm::vec3 blinnPhong(std::shared_ptr<intersect> hit);
    bool lightBlocked(const std::shared_ptr<Light> light, const std::shared_ptr<intersect> hit);

};

#endif
#include "Ray.h"

#include <iostream>
#include <algorithm>

using namespace std;
using namespace glm;

float Ray::self_t = 0.0001f;
vec3 Ray::camPos = vec3(0.0f);
vector<shared_ptr<Light>> Ray::lights = vector<shared_ptr<Light>>();
vector<shared_ptr<Object>> Ray::objects = vector<shared_ptr<Object>>();

Ray::Ray(glm::vec3 _p, glm::vec3 _v) :
    p(_p), v(glm::normalize(_v)), pixX(0), pixY(0), self(nullptr) {
};

Ray::Ray(glm::vec3 _p, glm::vec3 _v, std::shared_ptr<intersect> _self) :
    p(_p), v(glm::normalize(_v)), pixX(0), pixY(0), self(_self) {
};

Ray::Ray(glm::vec3 _p, glm::vec3 _v, int _pixX, int _pixY) :
    p(_p), v(glm::normalize(_v)), pixX(_pixX), pixY(_pixY), self(nullptr) {
};


shared_ptr<intersect> Ray::intersectTest(shared_ptr<intersect> _self, bool ignoreTrans) {
    auto intersects = std::vector<intersect>();
    for (auto obj : objects) {
        //if (!ignoreTrans || obj->shader != Object::TRANSPARENT)
        obj->intersectTest(p, v, intersects);
    }
    if (ignoreTrans) {
        for (int i = 0; i < intersects.size(); i++) {
            if (((Object*)intersects.at(i).obj)->shader == Object::TRANSPARENT) {
                intersects.erase(intersects.begin() + i);
            }
        }
    }
    if (intersects.size() == 0) { // no hits
        return nullptr;
    }
    int closest_i = -1;
    float closest_t = 1000.0f;
    for (int i = 0; i < intersects.size(); i++) {
        auto x = intersects.at(i);
        if (x.t < closest_t && x.t > 0.0f) {
            closest_i = i;
            closest_t = x.t;
        }
    }
    if (closest_i == -1) { // no hits
        return nullptr;
    } else {
        auto x = intersects.at(closest_i);
        return make_shared<intersect>(x.obj, x.t, x.pos, x.nor);
    }
}

vec3 Ray::trace() {
    auto hit = intersectTest(self);
    if (hit == nullptr) { // no hits
        return vec3(0.0f);
    } else {
        switch (((Object*)hit->obj)->shader) {
        case (Object::BLINN_PHONG):
        {
            return blinnPhong(hit);
        }
        break;
        case (Object::REFLECTIVE):
        {
            vec3 childV = v - 2 * dot(v, hit->nor) * hit->nor;
            auto childRay = Ray(hit->pos + self_t * childV, childV, hit);
            return childRay.trace();
        }
        break;
        case (Object::BLENDED):
        {
            vec3 childV = v - 2 * dot(v, hit->nor) * hit->nor;
            auto childRay = Ray(hit->pos + self_t * childV, childV, hit);
            auto rfColor = childRay.trace();
            auto bpColor = blinnPhong(hit);
            return rfColor * 0.3f + bpColor * 0.7f;
        }
        break;
        case (Object::TRANSPARENT):
        {
            auto childRay = Ray(hit->pos + self_t * v, v, hit);
            auto tpColor = childRay.trace();
            auto bpColor = blinnPhong(hit);
            return tpColor * 0.6f + bpColor * 0.4f;
        }
        break;
        }
    }
    return vec3(1.0f);
}

bool Ray::lightBlocked(const shared_ptr<Light> light, const shared_ptr<intersect> hit) {
    p = hit->pos;
    v = light->pos - p;
    float t = length(v);
    v = normalize(v);
    p += v * self_t;
    auto x = intersectTest(hit, true);
    if (x == nullptr)
        return true;
    else if (x->t > t)
        return true;
    else
        return false;
}

vec3 Ray::blinnPhong(shared_ptr<intersect> hit) {
    Object* obj = (Object*)hit->obj;
    auto x = hit->pos;
    auto n = hit->nor;

    vec3 eye = normalize(camPos - x);
    vec3 fragColor = obj->mat.ka;

    for (auto light : lights) {
        if (lightBlocked(light, hit)) {
            vec3 l = normalize(light->pos - x);
            vec3 h = normalize(l + eye);

            vec3 cd = obj->mat.kd * std::max(0.0f, (dot(l, n)));
            vec3 cs = obj->mat.ks * pow(std::max(0.0f, dot(h, n)), obj->mat.s);

            vec3 color = vec3(0.0f);
            color.r = light->color.r * (cd.r + cs.r);
            color.g = light->color.g * (cd.g + cs.g);
            color.b = light->color.b * (cd.b + cs.b);
            fragColor += color;
        }
    }
    fragColor.r = std::min(1.0f, fragColor.r);
    fragColor.g = std::min(1.0f, fragColor.g);
    fragColor.b = std::min(1.0f, fragColor.b);
    return fragColor;
}


#include "Plane.h"

using namespace std;
using namespace glm;

void Plane::intersectTest(const vec3& p, const vec3& v, vector<intersect>& intersects) {
    nor = normalize(nor);
    auto t = (dot(nor, (pos - p)) / dot(nor, v));
    auto x = p + t * v; // intersection
    intersects.push_back({ (void*)this, t, x, nor });
}
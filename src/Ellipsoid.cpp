#include "Ellipsoid.h"

using namespace std;
using namespace glm;

void Ellipsoid::intersectTest(const vec3& p, const vec3& v, vector<intersect>& intersects) {
    // ellipsoid_E is transformation matrix from unit sphere to ellipse
    // transform rays into local space for ellipse
    vec3 rayPrime[2];
    rayPrime[0] = inverse(ellipsoid_E) * vec4(p, 1.0f);
    rayPrime[1] = inverse(ellipsoid_E) * vec4(v, 0.0f);
    rayPrime[1] = normalize(rayPrime[1]);
    float a = dot(rayPrime[1], rayPrime[1]), b = 2 * dot(rayPrime[1], rayPrime[0]), c = dot(rayPrime[0], rayPrime[0]) - 1;
    float d = pow(b, 2) - 4 * a * c; // discriminant
    if (d > 0) { // two hits
        float tPrime = (-b + sqrt(d)) / (2 * a);
        auto xPrime = rayPrime[0] + tPrime * rayPrime[1];
        auto x = vec3(ellipsoid_E * vec4(xPrime, 1.0f));
        auto n = vec3(inverse(transpose(ellipsoid_E)) * vec4(xPrime, 0.0f));
        n = normalize(n);
        float t = length(x - p);
        if (dot(v, x - p) < 0) {// |t| always returns positive
            t = -t;
        }
        intersects.push_back({ (float*)this, t, x, n });

        tPrime = (-b - sqrt(d)) / (2 * a);
        xPrime = rayPrime[0] + tPrime * rayPrime[1];
        x = vec3(ellipsoid_E * vec4(xPrime, 1.0f));
        n = vec3(inverse(transpose(ellipsoid_E)) * vec4(xPrime, 0.0f));
        n = normalize(n);
        t = length(x - p);
        if (dot(v, x - p) < 0) {// |t| always returns positive
            t = -t;
        }
        intersects.push_back({ (void*)this, t, x, n });
    }
}

void Ellipsoid::calcE() {
    // Ellipsoid parameters
    auto M = make_shared<MatrixStack>();
    M->translate(pos);
    vec3 axis = normalize(vec3(1.0f, 1.0f, 1.0f));
    //M->rotate(1.0f, axis);
    M->scale(scale);
    ellipsoid_E = M->topMatrix();
}
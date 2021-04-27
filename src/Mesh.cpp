#include "Mesh.h"
#include "tri_intersect.h"

#include <iostream>

using namespace std;
using namespace glm;

void Mesh::loadMesh(const string& meshName) {
    Shape::loadMesh(meshName);
    calcBounds();
}

void Mesh::intersectTest(const vec3& p, const vec3& v, vector<intersect>& intersects) {
    auto boundIntersects = vector<intersect>();
    bounds->intersectTest(p, v, boundIntersects);
    bool testMesh = false;
    for (auto x : boundIntersects) {
        if (x.t > 0.0f) {
            testMesh = true;
            //intersects.push_back(x);
        }
    }
    if (testMesh) {
        vec3 rayPrimeP = inverse(mesh_E) * vec4(p, 1.0f);
        vec3 rayPrimeV = inverse(mesh_E) * vec4(v, 0.0f);
        rayPrimeV = normalize(rayPrimeV);
        vec3 posPrime = vec3(0.0f);
        vec3 norPrime = vec3(0.0f);
        for (int i = 0; i < (int)posBuf.size(); i += 9) {
            if (triangleIntersect(i, rayPrimeP, rayPrimeV, intersects, posPrime, norPrime)) {
                auto x = vec3(mesh_E * vec4(posPrime, 1.0f));
                auto n = vec3(inverse(transpose(mesh_E)) * vec4(norPrime, 0.0f));
                n = normalize(n);
                float t = length(x - p);
                if (dot(v, x - p) < 0) {// |t| always returns positive
                    t = -t;
                }
                intersects.push_back({ (void*)this, t, x, n });
            }
        }
    }
}

int Mesh::triangleIntersect(int i, vec3& p, vec3& v, vector<intersect>& intersects, vec3& hit, vec3& hitNor) {
    float orig[3] = { p.x, p.y, p.z };
    float dir[3] = { v.x, v.y, v.z };
    float vert0[3] = { posBuf[i], posBuf[i + 1], posBuf[i + 2] };
    float vert1[3] = { posBuf[i + 3], posBuf[i + 4], posBuf[i + 5] };
    float vert2[3] = { posBuf[i + 6], posBuf[i + 7], posBuf[i + 8] };
    float nor0[3] = { norBuf[i], norBuf[i + 1], norBuf[i + 2] };
    float nor1[3] = { norBuf[i + 3], norBuf[i + 4], norBuf[i + 5] };
    float nor2[3] = { norBuf[i + 6], norBuf[i + 7], norBuf[i + 8] };
    float t = 0;
    float baryU = 0;
    float baryV = 0;
    auto result = intersect_triangle3(orig, dir, vert2, vert0, vert1, &t, &baryU, &baryV);

    if (result == 1) {
        float baryW = 1 - (baryU + baryV);
        hit = vec3((baryU * vert0[0] + baryV * vert1[0] + baryW * vert2[0]),
            (baryU * vert0[1] + baryV * vert1[1] + baryW * vert2[1]),
            (baryU * vert0[2] + baryV * vert1[2] + baryW * vert2[2]));
        hitNor = vec3((baryU * nor0[0] + baryV * nor1[0] + baryW * nor2[0]),
            (baryU * nor0[1] + baryV * nor1[1] + baryW * nor2[1]),
            (baryU * nor0[2] + baryV * nor1[2] + baryW * nor2[2]));
        hitNor = normalize(hitNor);
        return 1;
    }
    return 0;
}

void Mesh::calcE() {
    // mesh parameters
    auto M = make_shared<MatrixStack>();
    M->translate(pos);
    M->rotate(rot.x, 1.0f, 0.0f, 0.0f);
    M->rotate(rot.y, 0.0f, 1.0f, 0.0f);
    M->rotate(rot.z, 0.0f, 0.0f, 1.0f);
    M->scale(scale);
    mesh_E = M->topMatrix();
}

void Mesh::calcBounds() {
    auto meanPos = vec3(0.0f);

    for (int i = 0; i < (int)posBuf.size(); i += 3) {
        auto point = vec3(posBuf[i], posBuf[i + 1], posBuf[i + 2]);
        meanPos += point;
    }
    meanPos /= (posBuf.size() / 3);
    auto farthestPoint = vec3(posBuf[0], posBuf[1], posBuf[2]);
    float farthestDist = length(meanPos - farthestPoint);

    for (int i = 0; i < (int)posBuf.size(); i += 3) {
        auto point = vec3(posBuf[i], posBuf[i + 1], posBuf[i + 2]);
        auto dist = length(meanPos - point);
        if (dist > farthestDist) {
            farthestPoint = point;
            farthestDist = dist;
        }
    }
    meanPos = vec3(mesh_E * vec4(meanPos, 1.0f));
    farthestPoint = vec3(mesh_E * vec4(farthestPoint, 1.0f));
    float radius = length(meanPos - farthestPoint);
    bounds = new Ellipsoid(meanPos, vec3(radius), vec3(0.0f), Material());
    bounds->shader = TRANSPARENT;
}
#define _USE_MATH_DEFINES
#include <cmath> 
#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "MatrixStack.h"

using namespace std;


Camera::Camera() :
	pos(0.0f, 0.0f, 5.0f),
	forward(0.0f, 0.0f, -1.0f),
	aspect(1.0f),
	fovy((float)glm::radians(45.0)),
	znear(0.1f),
	zfar(1000.0f) {
}

Camera::~Camera() {
}

void Camera::applyProjectionMatrix(std::shared_ptr<MatrixStack> P) const {
	// Modify provided MatrixStack
	P->multMatrix(glm::perspective(fovy, aspect, znear, zfar));
}

glm::mat4 Camera::getProjectionMatrix() {
	return glm::perspective(fovy, aspect, znear, zfar);
}

void Camera::applyViewMatrix(std::shared_ptr<MatrixStack> MV) {
	glm::mat4 view = getViewMatrix();
	MV->multMatrix(view);
}

glm::mat4 Camera::getViewMatrix() {
	glm::mat4 view = glm::lookAt(pos, pos + forward, glm::vec3(0.0f, 1.0f, 0.0f));
	return view;
}

void Camera::makeRays(int width, int height, shared_ptr<vector<Ray>> rays) {
	////////////////
	// L13 Method //
	////////////////
	Ray::camPos = pos;
	setAspect((float)width / height);
	auto P = getProjectionMatrix();
	auto C = glm::inverse(getViewMatrix());

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			float xmouse = x + 0.5f;
			float ymouse = y + 0.5f;

			auto Pn = glm::vec2(2.0f * xmouse / width - 1, 2.0f * ymouse / height - 1); // pixel -> normalized device coord (y is inverted)
			auto Pc = glm::vec4(Pn.x, Pn.y, -1, 1); // -> clip coord
			auto Pe = glm::inverse(P) * Pc; // -> eye coord
			Pe.w = 1.0f;
			auto Pw = C * Pe; // -> world coord
			auto rayV = glm::vec3(Pw) - pos; // Pw is a point, we want direction from camera position
			//rayV = glm::normalize(rayV);

			rays->push_back(Ray(pos, rayV, x, y));
			//std::cout << "Ray direction for (" << x << ", " << y << "): (" << rayV.x << " " << rayV.y << " " << rayV.z << ")" << std::endl;
		}
	}
}

void Camera::printPos() {
	if (false)
		std::cout << "Camera postion: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
}



#pragma  once
#ifndef CAMERA_H
#define CAMERA_H

#include <memory>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Ray.h"

class MatrixStack;

class Camera {
public:
	glm::vec3 pos;
	glm::vec3 forward;

	Camera();
	virtual ~Camera();
	void makeRays(int width, int height, std::shared_ptr<std::vector<Ray>> rays);

	void applyProjectionMatrix(std::shared_ptr<MatrixStack> P) const;
	glm::mat4 getProjectionMatrix();
	void applyViewMatrix(std::shared_ptr<MatrixStack> MV);
	glm::mat4 getViewMatrix();
	void setPos(glm::vec3 p) { pos = p; };
	void setForward(glm::vec3 fw) { forward = fw; };
	void setAspect(float a) { aspect = a; };
	void setFov(float f) { fovy = f; };
	void printPos();

private:
	float aspect;
	float fovy;
	float znear;
	float zfar;
};

#endif

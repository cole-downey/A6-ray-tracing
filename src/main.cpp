#include <iostream>
#include <string>
#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Ray.h"
#include "Camera.h"
#include "MatrixStack.h"
#include "Shape.h"
#include "Material.h"
#include "Light.h"
#include "Object.h"
#include "Ellipsoid.h"
#include "Plane.h"
#include "Mesh.h"

#include "Image.h"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;

int main(int argc, char** argv) {
	if (argc < 2) {
		cout << "Usage: A6 <scene>" << endl;
		return 0;
	}
	//string meshName(argv[1]);

	//////////////////
	// Main program // 
	//////////////////

	string outName = "../resources/result.png";
	string bunnyName = "../resources/bunny.obj";

	int size = 512;
	int width = size, height = size;
	auto image = make_shared<Image>(width, height);

	auto camera = Camera();
	auto mat = Material();

	int scene = atoi(argv[1]);
	switch (scene) {
	case 0:
		// lights
		Ray::lights.push_back(make_shared<Light>(glm::vec3(-2.0f, 1.0f, 1.0f), glm::vec3(1.0f)));
		// objects
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		Ray::objects.push_back(make_shared<Ellipsoid>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f), mat));
		break;
	case 1:
	case 2:
		// lights
		Ray::lights.push_back(make_shared<Light>(glm::vec3(-2.0f, 1.0f, 1.0f), glm::vec3(1.0f)));
		// objects
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		Ray::objects.push_back(make_shared<Ellipsoid>(glm::vec3(-0.5f, -1.0f, 1.0f), glm::vec3(1.0f), glm::vec3(0.0f), mat));
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		Ray::objects.push_back(make_shared<Ellipsoid>(glm::vec3(0.5f, -1.0f, -1.0f), glm::vec3(1.0f), glm::vec3(0.0f), mat));
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		Ray::objects.push_back(make_shared<Ellipsoid>(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f), mat));
		break;
	case 3:
		// lights
		Ray::lights.push_back(make_shared<Light>(glm::vec3(1.0f, 2.0f, 2.0f), glm::vec3(0.5f)));
		Ray::lights.push_back(make_shared<Light>(glm::vec3(-1.0f, 2.0f, -1.0f), glm::vec3(0.5f)));
		// objects
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		Ray::objects.push_back(make_shared<Ellipsoid>(glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(0.5f, 0.6f, 0.2f), glm::vec3(0.0f), mat));
		Ray::objects.back()->shader = Object::TRANSPARENT;
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		Ray::objects.push_back(make_shared<Ellipsoid>(glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(1.0f), glm::vec3(0.0f), mat));
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, 0.0f);
		Ray::objects.push_back(make_shared<Plane>(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), mat));
		break;
	case 4:
	case 5:
		// lights
		Ray::lights.push_back(make_shared<Light>(glm::vec3(-1.0f, 2.0f, 1.0f), glm::vec3(0.5f)));
		Ray::lights.push_back(make_shared<Light>(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.5f)));
		// objects
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		Ray::objects.push_back(make_shared<Ellipsoid>(glm::vec3(0.5f, -0.7f, 0.5f), glm::vec3(0.3f), glm::vec3(0.0f), mat));
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		Ray::objects.push_back(make_shared<Ellipsoid>(glm::vec3(1.0f, -0.7f, 0.0f), glm::vec3(0.3f), glm::vec3(0.0f), mat));
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		Ray::objects.push_back(make_shared<Ellipsoid>(glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(1.0f), glm::vec3(0.0f), mat));
		Ray::objects.back()->shader = Object::REFLECTIVE;
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		Ray::objects.push_back(make_shared<Ellipsoid>(glm::vec3(1.5f, 0.0f, -1.5f), glm::vec3(1.0f), glm::vec3(0.0f), mat));
		Ray::objects.back()->shader = Object::REFLECTIVE;
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, 0.0f);
		Ray::objects.push_back(make_shared<Plane>(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), mat));
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, 0.0f);
		Ray::objects.push_back(make_shared<Plane>(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 1.0f), mat));
		break;
	case 6:
		// lights
		Ray::lights.push_back(make_shared<Light>(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(1.0f)));
		// objects
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		{shared_ptr<Mesh> bunny = make_shared<Mesh>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f), mat);
		bunny->loadMesh(bunnyName);
		Ray::objects.push_back(bunny);}
		break;
	case 7:
		// lights
		Ray::lights.push_back(make_shared<Light>(glm::vec3(1.0f, 1.0f, 2.0f), glm::vec3(1.0f)));
		// objects
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		{shared_ptr<Mesh> bunny = make_shared<Mesh>(glm::vec3(0.3f, -1.5f, 0.0f), glm::vec3(1.5f), glm::vec3(glm::radians(20.0f), 0.0f, 0.0f), mat);
		bunny->loadMesh(bunnyName);
		Ray::objects.push_back(bunny);}
		break;
	case 8:
		// camera transforms
		camera.setPos({ -3.0f, 0.0f, 0.0f });
		camera.setForward({ 1.0f, 0.0f, 0.0f });
		camera.setFov((float)glm::radians(60.0f));
		// lights
		Ray::lights.push_back(make_shared<Light>(glm::vec3(-2.0f, 1.0f, 1.0f), glm::vec3(1.0f)));
		// objects
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		Ray::objects.push_back(make_shared<Ellipsoid>(glm::vec3(-0.5f, -1.0f, 1.0f), glm::vec3(1.0f), glm::vec3(0.0f), mat));
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		Ray::objects.push_back(make_shared<Ellipsoid>(glm::vec3(0.5f, -1.0f, -1.0f), glm::vec3(1.0f), glm::vec3(0.0f), mat));
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		Ray::objects.push_back(make_shared<Ellipsoid>(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f), mat));
		break;
	case 9:
		// lights
		Ray::lights.push_back(make_shared<Light>(glm::vec3(0.5f, -0.5f, 2.5f), glm::vec3(0.75f)));
		Ray::lights.push_back(make_shared<Light>(glm::vec3(-1.0f, 0.7f, 2.5f), glm::vec3(0.25f)));
		// objects
		// sphere
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		Ray::objects.push_back(make_shared<Ellipsoid>(glm::vec3(0.0f, 0.7f, 2.0f), glm::vec3(0.2f), glm::vec3(0.0f), mat));
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		Ray::objects.push_back(make_shared<Ellipsoid>(glm::vec3(0.75f, -0.4f, 1.0f), glm::vec3(0.6f), glm::vec3(0.0f), mat));
		Ray::objects.back()->shader = Object::BLENDED;
		// bunny
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 0.5f, 0.5f, 1.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		{shared_ptr<Mesh> bunny = make_shared<Mesh>(glm::vec3(-0.3f, -1.13f, 0.0f), glm::vec3(1.0f), glm::vec3(glm::radians(20.0f), 0.0f, 0.0f), mat);
		bunny->loadMesh(bunnyName);
		Ray::objects.push_back(bunny);}
		Ray::objects.back()->shader = Object::BLENDED;
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, 0.0f);
		Ray::objects.push_back(make_shared<Plane>(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), mat));
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, 0.0f);
		Ray::objects.push_back(make_shared<Plane>(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 1.0f), mat));
		break;
	}

	// generate rays
	auto rays = make_shared<vector<Ray>>();
	camera.makeRays(width, height, rays);
	cout << "Ray creation done" << endl;

	// Trace each ray
	for (auto ray : *rays) {
		image->setPixel(ray.pixX, ray.pixY, ray.trace());
	}
	cout << "Ray tracing done" << endl;

	// Write image to file
	image->writeToFile(outName);
	return 0;
}

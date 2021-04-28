#include <iostream>
#include <iomanip>
#include <string>
#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <windows.h>
#include <time.h>
#include <chrono>

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

shared_ptr<vector<Ray>> rays;
shared_ptr<Image> image;

struct rayThreadArgs {
	int startRay;
	int endRay;
};
DWORD WINAPI rayThread(LPVOID lpParameter) {
	auto args = *(rayThreadArgs*)lpParameter;
	delete lpParameter;
	for (int i = args.startRay; i < args.endRay; i++) {
		image->setPixel(rays->at(i).pixX, rays->at(i).pixY, rays->at(i).trace());
	}
	return 0;
};

int main(int argc, char** argv) {
	if (argc < 3) {
		cout << "Usage: A6 <scene> <size> <nthreads>(opt) <imageName>(opt)" << endl;
		return 0;
	}
	string outName = "../results/result.png";
	int nthreads = 0;
	if (argc > 4) outName = string(argv[4]);
	if (argc > 3) nthreads = atoi(argv[3]);

	//////////////////
	// Main program // 
	//////////////////

	string bunnyName = "../resources/bunny.obj";

	int size = atoi(argv[2]);
	int width = size, height = size;
	image = make_shared<Image>(width, height);

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
		Ray::lights.push_back(make_shared<Light>(glm::vec3(-1.0f, 2.0f, 1.0f), glm::vec3(0.5f)));
		Ray::lights.push_back(make_shared<Light>(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.5f)));
		// objects
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		Ray::objects.push_back(make_shared<Ellipsoid>(glm::vec3(0.5f, -0.7f, 0.5f), glm::vec3(0.3f), glm::vec3(0.0f), mat));
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		Ray::objects.push_back(make_shared<Ellipsoid>(glm::vec3(1.0f, -0.7f, 0.0f), glm::vec3(0.3f), glm::vec3(0.0f), mat));
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		Ray::objects.push_back(make_shared<Ellipsoid>(glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(1.0f), glm::vec3(0.0f), mat));
		Ray::objects.back()->shader = Object::BLENDED;
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.5f }, 100.0f);
		Ray::objects.push_back(make_shared<Ellipsoid>(glm::vec3(1.5f, 0.0f, -1.5f), glm::vec3(1.0f), glm::vec3(0.0f), mat));
		Ray::objects.back()->shader = Object::BLENDED;
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, 0.0f);
		Ray::objects.push_back(make_shared<Plane>(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), mat));
		mat = Material({ 0.1f, 0.1f, 0.1f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, 0.0f);
		Ray::objects.push_back(make_shared<Plane>(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 1.0f), mat));
		break;
	case 10:
		// lights
		Ray::lights.push_back(make_shared<Light>(glm::vec3(-1.0f, 0.7f, 2.5f), glm::vec3(0.25f)));
		Ray::lights.push_back(make_shared<Light>(glm::vec3(0.5f, -0.5f, 2.5f), glm::vec3(0.75f)));
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
	rays = make_shared<vector<Ray>>();
	camera.makeRays(width, height, rays);
	cout << "Ray creation done." << endl;

	// start timer
	auto t_start = chrono::high_resolution_clock::now();

	// Trace each ray
	if (nthreads > 1) {
		int nrays = (int)rays->size();
		auto threads = make_shared<vector<HANDLE>>();
		int pixPerThread = nrays / nthreads;
		for (int i = 0; i < nrays; i += pixPerThread) {
			int start = i, stop = i + pixPerThread;
			if (stop > nrays) stop = nrays;
			auto args = new rayThreadArgs{
				start,
				stop,
			};
			threads->push_back(CreateThread(0, 0, rayThread, args, 0, 0));
		}
		cout << "Ray threads started..." << endl;
		for (auto thread : *threads) {
			WaitForSingleObject(thread, INFINITE);
			CloseHandle(thread);
		}
	} else {
		for (auto ray : *rays) {
			image->setPixel(ray.pixX, ray.pixY, ray.trace());
		}
	}

	// stop timer
	auto t_end = chrono::high_resolution_clock::now();
	cout << "Ray tracing done." << endl;
	double timediff = chrono::duration<double, std::milli>(t_end - t_start).count();
	cout << "Time elapsed: ";
	cout << setfill('0') << setw(2) << ((int)timediff / (1000 * 60 * 60)) % 60 << ":" << setw(2) << ((int)timediff / (1000 * 60)) % 60 << ":";
	cout << setw(2) << ((int)timediff / (1000)) % 60 << ".";
	cout << setw(2) << (int)(timediff / 10) % 100 << endl;

	// Write image to file
	image->writeToFile(outName);
	return 0;
}

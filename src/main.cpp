#include "scene.h"
#include <iostream>
int main() {
	std::cout << "start";
	RaycastScene rs;
	rs.registerCamera(new PerspectiveCamera({ 0, 0.25, 1 }, { 0, 0, -1 }, { 0, 1, 0 }, 200, 200, M_PI / 6));
	rs.registerPresenter(new BmpPresenter());
	// rs.addEntity(new Sphere({ -4, 4, 0 }, 1, new PhongMaterial({ 0, 1, 0 }, 3)));
	// rs.addEntity(new Sphere({ 4, -4, 0 }, 1, new PhongMaterial({ 0, 1, 0 }, 20)));
	// rs.addEntity(new Plain({0, 1, 0}, -2, new LambertMaterial(Color(1))));
	// rs.addEntity(new Triangle({-4, 4, 0}, {4, 4, 0}, {4, -4, 0}, new PhongMaterial(Color(1), 20)));
	// rs.addEntity(new Triangle({4, -4, 20}, {4, -4, 0}, {4, 4, 0}, new PhongMaterial(Color(1), 20)));
	//rs.addEntity(new Plain({1, 0, 0}, 4, new PhongMaterial(Color(1), 20)));
//	for (int i = 0; i < 100; i++) {
	// rs.addLight(new PointLight({ -2, 3, 1 }, Color(1), 0.4));
//	}
	rs.addEntity(new Mesh("bunny_200.obj", Vector3f(), new PhongMaterial(Color(1), 20)));
	rs.addLight(new DirectionalLight({0, 1, -1}, Color(1), 0.5));
	rs.registerDetector(new BruteForceDetector());
	rs.sceneComplete();
	rs.render("./1.bmp");
	std::cout << "end";
}

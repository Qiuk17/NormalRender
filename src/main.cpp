#include "scene.h"

int main() {
	RaycastScene rs;
	rs.registerCamera(new PerspectiveCamera({ 0, 0, 20 }, { 0, 0, -1 }, { 0, 1, 0 }, 1000, 1000, M_PI / 6));
	rs.registerPresenter(new BmpPresenter());
	rs.addEntity(new Sphere({ -1, 1, 0 }, 1, new PhongMaterial({ 0, 1, 0 }, 3)));
	rs.addEntity(new Sphere({ 1, -1, 0 }, 1, new PhongMaterial({ 0, 1, 0 }, 20)));
	rs.addEntity(new Sphere({ 0, 0, 10 }, 15, new PhongMaterial({ 0, 1, 0 }, 0)));
	rs.addLight(new PointLight({ 0, 0, 0 }, Color(1), 0.4));
	rs.addLight(new DirectionalLight({0, 1, -1}, Color(1), 0.1));
	rs.registerDetector(new BruteForceDetector());
	rs.sceneComplete();
	rs.render("./1.bmp");
}

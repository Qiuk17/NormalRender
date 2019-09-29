#include "manager.h"

int main() {
	RaycastScene rs;
	rs.registerCamera(new PerspectiveCamera({ 0, 0, 10 }, { 0, 0, -1 }, { 0, 1, 0 }, 200, 200, M_PI / 6));
	rs.registerPresenter(new BmpPresenter());
	rs.addEntity(new Sphere({ 0, 0, 0 }, 1, new PhongMaterial({ 0, 1, 0 }, 20)));
	rs.addLight(new PointLight({ 0, 5, 5 }, Color(1), 0.9));
	rs.sceneComplete();
	rs.render("./1.bmp");
}

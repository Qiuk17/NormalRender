#include "scene.h"
#include <iostream>
#include <cassert>
#include <GL/glut.h>

AbstractScene::~AbstractScene() {
	for (auto p : vecLight) delete p;
	for (auto p : vecMaterial) delete p;
	if (pPresenter != nullptr) delete pPresenter;
	if (pCamera != nullptr) delete pCamera;
	if (pGroup != nullptr) delete pGroup;
}

RaycastScene::~RaycastScene() {
	for (auto p : vecLight) delete p;
	for (auto p : vecMaterial) delete p;
	if (pPresenter != nullptr) delete pPresenter;
	if (pCamera != nullptr) delete pCamera;
	if (pLights != nullptr) delete pLights;
	if (pGroup != nullptr) delete pGroup;
}

void RaycastScene::sceneComplete() {
	if (pPresenter == nullptr) std::__throw_runtime_error("Presenter is not assigned.");
	if (pCamera == nullptr) std::__throw_runtime_error("Camera is not assigned.");
	if (pGroup == nullptr) std::__throw_runtime_error("Group is not assigned.");
	countLight = vecLight.size();
	pLights = new Light * [countLight];
	int top = 0;
	for (auto p : vecLight) pLights[top++] = p;
	pPresenter->init(pCamera->getWidth(), pCamera->getHeight());
}

void RaycastScene::render(int* argc, char** argv) {
	int width = pCamera->getWidth(), height = pCamera->getHeight();
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			auto ray = pCamera->generateRay(x, y);
			auto col = pGroup->interact(ray);
			if (!col.isValid) { pPresenter->setPixel(x, y, background); continue; }
			if (col.pEntity->getMaterial() == nullptr) { pPresenter->setPixel(x, y, Color(1)); continue; }
			auto color = Color();
			for (int i = 0; i < countLight; i++) {
				auto li = pLights[i]->castOnPoint(col.point);
				if (Vector3f::dot(li.direction, col.normal) > 0/* && !pGroup->getDetector()->isRayCollided(Ray(col.point + col.normal * 1e-4, li.direction), -1, li.distance + 1e-3)*/) {
					color += col.pEntity->getMaterial()->getColor(col, *pLights[i]).restriction();
					//color = Color(1) * Vector3f::dot(col.normal, -ray.direction);
				}
				//else if (pGroup->getDetector()->isRayCollided(Ray(col.point + col.normal * 1e-4, li.direction), -1, li.distance + 1e-3)) color = Color(1, 0, 0);
			}
			color.restricted();
			pPresenter->setPixel(x, y, color);
		}
	}
	pPresenter->finish(argv[2]);
}

OpenGlScene::~OpenGlScene() {
	for (auto p : vecLight) delete p;
	for (auto p : vecMaterial) delete p;
	if (pPresenter != nullptr) delete pPresenter;
	if (pCamera != nullptr) delete pCamera;
	if (pGroup != nullptr) delete pGroup;
}

void OpenGlScene::sceneComplete() {
	if (pPresenter == nullptr) std::__throw_runtime_error("Presenter is not assigned.");
	if (pCamera == nullptr) std::__throw_runtime_error("Camera is not assigned.");
	if (pGroup == nullptr) std::__throw_runtime_error("Group is not assigned.");
	pPresenter->init(pCamera->getWidth(), pCamera->getHeight());
}

void OpenGlScene::render(int* argc, char** argv) {
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(60, 60);
	glutInitWindowSize(pCamera->getWidth(), pCamera->getHeight());
	glutCreateWindow("OpenGL");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(background.r, background.g, background.b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pCamera->glSetup();
	int cnt = 0;
	for (auto p : vecLight) p->glTurnOn(cnt++);
	pGroup->glDraw();
	glutSwapBuffers();
	int width = pCamera->getWidth(), height = pCamera->getHeight();
	auto bufferDump = new unsigned char[3 * width * height];
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadBuffer(GL_BACK);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, bufferDump);
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			pPresenter->setPixel(x, height - 1 - y, Color(
				bufferDump[3 * (y * width + x) + 0] / 255.0,
				bufferDump[3 * (y * width + x) + 1] / 255.0,
				bufferDump[3 * (y * width + x) + 2] / 255.0));
		}
	}
	delete bufferDump; pPresenter->finish(argv[2]);
}

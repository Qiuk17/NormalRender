#include "manager.h"
#include <iostream>
AbstractScene::~AbstractScene() {
	for (auto p : vecEntity) delete p;
	for (auto p : vecLight) delete p;
	for (auto p : vecMaterial) delete p;
	if (pPresenter != nullptr) delete pPresenter;
	if (pCamera != nullptr) delete pCamera;
	if (pLights != nullptr) delete pLights;
	if (pDetector != nullptr) delete pDetector;
}

void AbstractScene::sceneComplete() {
	if (pPresenter == nullptr) throw std::exception();
	if (pCamera == nullptr) throw std::exception();
	if (pDetector == nullptr) throw std::exception();
	countLight = vecLight.size();
	pLights = new Light * [countLight];
	int top = 0;
	for (auto p : vecLight) pLights[top++] = p;
	pPresenter->init(pCamera->getWidth(), pCamera->getHeight());
	pDetector->prepareDetector(vecEntity);
}

void RaycastScene::render(const char* finishArgv) {
	int width = pCamera->getWidth(), height = pCamera->getHeight();
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			auto ray = pCamera->generateRay(x, y);
			auto col = pDetector->getClosestCollision(ray);
			if (!col.isValid) { pPresenter->setPixel(x, y, background); continue; }
			if (col.pEntity->getMaterial() == nullptr) { pPresenter->setPixel(x, y, Color(1)); continue; }
			auto color = Color();
			for (int i = 0; i < countLight; i++) {
				auto li = pLights[i]->castOnPoint(col.point);
				if (Vector3f::dot(li.direction, col.normal) > 0 && !pDetector->isRayCollided(Ray(col.point + li.direction, li.direction), 1e-3f, li.distance))
					color += col.pEntity->getMaterial()->getColor(col, *pLights[i]).restriction();
			}
			color.restricted();
			pPresenter->setPixel(x, y, color);
		}
	}
	pPresenter->finish(finishArgv);
}

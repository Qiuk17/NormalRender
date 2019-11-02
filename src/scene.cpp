#include "scene.h"
#include <iostream>
#include <cassert>
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

void RaycastScene::render(const char* finishArgv) {
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
	pPresenter->finish(finishArgv);
}

#include "manager.h"

AbstractScene::~AbstractScene() {
	for (auto p : vecEntity) delete p;
	for (auto p : vecLight) delete p;
	for (auto p : vecMaterial) delete p;
	if (pPresenter != nullptr) delete pPresenter;
	if (pCamera != nullptr) delete pCamera;
	if (pEntities != nullptr) delete pEntities;
	if (pLights != nullptr) delete pLights;
	if (pMaterials != nullptr) delete pMaterials;
}

void AbstractScene::sceneComplete() {
	if (pPresenter == nullptr) throw std::exception("No presenter is provided. No result can be produced.");
	if (pCamera == nullptr) throw std::exception("No camera is provided. Nothing can be seen.");
	countEntity = vecEntity.size();
	countLight = vecLight.size();
	countMaterial = vecMaterial.size();
	pEntities = new Entity * [countEntity];
	pLights = new Light * [countLight];
	pMaterials = new Material * [countMaterial];
	int top = 0;
	for (auto p : vecEntity) pEntities[top++] = p;
	top = 0;
	for (auto p : vecLight) pLights[top++] = p;
	top = 0;
	for (auto p : vecMaterial) pMaterials[top++] = p;
	vecEntity.clear(); vecLight.clear(); vecMaterial.clear();

	pPresenter->init(pCamera->getWidth(), pCamera->getHeight());
}

void RaycastScene::render(const char* finishArgv) {
	int width = pCamera->getWidth(), height = pCamera->getHeight();
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			auto ray = pCamera->generateRay(x, y);
			auto col = Collision(&ray, false);
			for (int i = 0; i < countEntity; i++) {
				auto nc = pEntities[i]->interact(ray);
				if (!nc.isValid) continue;
				if (!col.isValid) { col = nc; continue; }
				col = col.distance < nc.distance ? col : nc;
			}
			if (!col.isValid) { pPresenter->setPixel(x, y, background); continue; }
			if (col.pEntity->getMaterial() == nullptr) { pPresenter->setPixel(x, y, Color(1)); continue; }
			auto color = Color();
			for (int i = 0; i < countLight; i++) {
				auto li = pLights[i]->castOnPoint(col.point);
				for (int j = 0; j < countEntity; j++) {
					auto col2 = pEntities[j]->interact(Ray(col.point, li.direction));
					if (col2.isValid && col2.distance >= 0.000001) continue;
					color += col.pEntity->getMaterial()->getColor(col, *pLights[i]).restriction();
				}
			}
			color.restricted();
			pPresenter->setPixel(x, y, color);
		}
	}
	pPresenter->finish(finishArgv);
}

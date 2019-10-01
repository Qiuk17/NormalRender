#ifndef _MANAGER_H
#define _MANAGER_H

#include <vector>

#include "materials.h"
#include "entities.h"
#include "light.h"
#include "color.h"
#include "camera.h"
#include "presenter.h"
#include "detector.h"

class AbstractScene {
public:
	AbstractScene() = default;
	virtual ~AbstractScene();
	virtual void sceneComplete();
    virtual void render(const char* finishArgv = nullptr) = 0;
	void addMaterial(Material* pMaterial) { vecMaterial.emplace_back(pMaterial); }
	void addEntity(Entity* pEntity) { vecEntity.emplace_back(pEntity); }
	void addLight(Light* pLight) { vecLight.emplace_back(pLight); }
	void setBackgroundColor(const Color& color) { background = color; }
	void registerCamera(Camera* pCamera_) { if (pCamera != nullptr) delete pCamera; pCamera = pCamera_; }
	void registerPresenter(Presenter* pPresenter_) { if (pPresenter != nullptr) delete pPresenter; pPresenter = pPresenter_; }
	void registerDetector(Detector* pDetector_) {if (pDetector != nullptr) delete pDetector; pDetector = pDetector_;}
	
protected:
    std::vector<Material*> vecMaterial;
    std::vector<Entity*>   vecEntity;
    std::vector<Light*>    vecLight;
    Light**    pLights = nullptr;
    Camera*    pCamera = nullptr;
    Presenter* pPresenter = nullptr;
	Detector* pDetector = nullptr;
    int countLight = 0;
	Color background = Color();
};

class RaycastScene : public AbstractScene {
public:
	void render(const char* finishArgv = nullptr) override;
};

#endif

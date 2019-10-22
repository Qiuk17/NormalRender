#ifndef _SCENE_H
#define _SCENE_H

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
	virtual void sceneComplete() = 0;
    virtual void render(const char* finishArgv = nullptr) = 0;
	void addMaterial(Material* pMaterial) { vecMaterial.emplace_back(pMaterial); }
	void addLight(Light* pLight) { vecLight.emplace_back(pLight); }
	void setBackgroundColor(const Color& color) { background = color; }
	void registerGroup(Group* pGroup_) { if (pGroup != nullptr) delete pGroup; pGroup = pGroup_; }
	void registerCamera(Camera* pCamera_) { if (pCamera != nullptr) delete pCamera; pCamera = pCamera_; }
	void registerPresenter(Presenter* pPresenter_) { if (pPresenter != nullptr) delete pPresenter; pPresenter = pPresenter_; }
	Material* getMaterial(int index) const {return vecMaterial[index]; }
	Entity* getEntity(int index) const { return pGroup->getEntity(index); }
	Light* getLight(int index) const { return vecLight[index]; }
protected:
    std::vector<Material*> vecMaterial;
    std::vector<Light*>    vecLight;
	Group*     pGroup = nullptr;
    Camera*    pCamera = nullptr;
    Presenter* pPresenter = nullptr;
	Color background = Color();
};

class RaycastScene : public AbstractScene {
public:
	~RaycastScene() override;
	void sceneComplete() override;
	void render(const char* finishArgv = nullptr) override;
private:
	Light** pLights = nullptr;
	int countLight = 0;
};

#endif

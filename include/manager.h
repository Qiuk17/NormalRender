#ifndef _MANAGER_H
#define _MANAGER_H

#include <vector>

#include "materials.h"
#include "entities.h"
#include "light.h"
#include "color.h"
#include "camera.h"
#include "presenter.h"

class ResourceManager {
public:
    void render();
private:
    std::vector<Material*> vecMaterial;
    std::vector<Entity*>   vecEntity;
    std::vector<Light*>    vecLight;
    Material** pMaterials = nullptr;
    Entity**   pEntities = nullptr;
    Light**    pLights = nullptr;
    Camera*    pCamera = nullptr;
    Presenter* pPresenter = nullptr;
    int countMaterial = 0, countEntity = 0, countLight = 0;
};

#endif

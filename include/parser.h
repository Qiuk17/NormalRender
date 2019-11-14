#ifndef _PARSER_H
#define _PARSER_H

#include <string>
#include <fstream>
#include <map>
#include <vecmath.h>
#include "scene.h"

enum SCENE_TYPE {
    ST_RAYCAST,
    ST_OPENGL,
    ST_RAYTRACE,
    ST_PHOTONMAPPING
};

enum PRESENTER_TYPE {
    PT_IMAGE
};

enum DETECTOR_TYPE {
    DT_BRUTEFORCE,
    DT_KDTREE
};

class Parser {
public:
    static AbstractScene* CreateSceneFromFile(const char* sceneFilePath,
    SCENE_TYPE type = ST_RAYCAST,
    PRESENTER_TYPE pType = PT_IMAGE) {
        Parser parser(sceneFilePath, type, pType);
        return parser.parse();
    }

private:
    std::ifstream stream;
    AbstractScene* pScene = nullptr;
    Parser(const char* sceneFilePath, SCENE_TYPE type, PRESENTER_TYPE pType) : stream(sceneFilePath) {
        if (!stream.good()) std::__throw_ios_failure("[Error] Unable to open the scene file.");
        switch (type) {
            case ST_RAYCAST: pScene = new RaycastScene(); break;
            case ST_OPENGL: pScene = new OpenGlScene(); break;
            case ST_RAYTRACE:
            case ST_PHOTONMAPPING: std::__throw_logic_error("[Error] Unimplemented Scene.");
            default: std::__throw_invalid_argument("[Error] Bad scene type.");
        }
        switch (pType) {
            case PT_IMAGE: pScene->registerPresenter(new BmpPresenter()); break;
            default: std::__throw_invalid_argument("[Error] Bad presenter type.");
        }
    }
    AbstractScene* parse();
    void parsePerspectiveCamera();
    void parseBackground();
    void parseLights();
    void parseDirectionalLight();
    void parsePointLight();
    void parseMaterials();
    void parseDiffMaterial();
    void parsePhongMaterial();
    void parseTopGroup();
    Group* parseGroup();
    Material* parseMaterialFromIndex();
    Entity* parseEntity(const std::string& token, Material* material);
    Sphere* parseSphere(Material* material);
    Plane* parsePlane(Material* material);
    Triangle* parseTriangle(Material* material);
    Mesh* parseMesh(Material* material);
    Transform* parseTransform(Material* material);
    Curve* parseBsplineCurve(Material* matetial);
    Curve* parseBezierCurve(Material* material);
    RevCurveSurface* parseRevCurveSurface(Material* material);
    bool getToken(std::string& token) {
        stream >> token;
        if (stream.eof()) return false;
        return true;
    }
    void testToken(const char* c_token) {
        std::string token;
        getToken(token);
        if (token != c_token) std::__throw_logic_error("[Error] Syntax error.");
    }

};

#endif

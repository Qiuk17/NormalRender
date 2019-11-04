#include "parser.h"
#include <cstring>
#define BRACE_BEGIN() testToken("{")
#define BRACE_END()   testToken("}")
#define GET(name,type) type name; stream >> name; ((void) 0)
#define GET_PARAMETER(name,type) testToken(#name); type name; (stream >> name); ((void) 0)
#define DEGREE_TO_RADIANS(x) ((M_PI * x) / 180.0f)
#define _THROW_SYNTAX_ERROR(file,line) do {std::string error = "[Error] Syntax Error.\n"; error += file ":"; error += #line;std::__throw_logic_error(error.c_str());} while(0)
#define THROW_SYNTAX_ERROR(file,line) _THROW_SYNTAX_ERROR(file,line)
AbstractScene* Parser::parse() {
    std::string token;
    while (getToken(token)) {
        if (token == "PerspectiveCamera") {
            parsePerspectiveCamera();
        } else if (token == "Background") {
            parseBackground();
        } else if (token == "Lights") {
            parseLights();
        } else if (token == "Materials") {
            parseMaterials();
        } else if (token == "Group") {
            parseTopGroup();
        } else {
            THROW_SYNTAX_ERROR(__FILE__, __LINE__);
        }
    }
    pScene->sceneComplete();
    return pScene;
}

void Parser::parsePerspectiveCamera() {
    BRACE_BEGIN();
    GET_PARAMETER(center, Vector3f);
    GET_PARAMETER(direction, Vector3f);
    GET_PARAMETER(up, Vector3f);
    GET_PARAMETER(angle, float);
    GET_PARAMETER(width, int);
    GET_PARAMETER(height, int);
    BRACE_END();
    pScene->registerCamera(new PerspectiveCamera(center, direction, up, width, height, DEGREE_TO_RADIANS(angle)));
}

void Parser::parseBackground() {
    BRACE_BEGIN();
    GET_PARAMETER(color, Color);
    BRACE_END();
    pScene->setBackgroundColor(color);
}

void Parser::parseLights() {
    std::string token;
    BRACE_BEGIN();
    GET_PARAMETER(numLights, int);
    for (int i = 0; i < numLights; i++) {
        getToken(token);
        if (token == "DirectionalLight")
            parseDirectionalLight();
        else if (token == "PointLight")
            parsePointLight();
        else THROW_SYNTAX_ERROR(__FILE__, __LINE__);
    }
    BRACE_END();
}

void Parser::parseDirectionalLight() {
    BRACE_BEGIN();
    GET_PARAMETER(direction, Vector3f);
    GET_PARAMETER(color, Color);
    BRACE_END();
    pScene->addLight(new DirectionalLight(direction, color));
}

void Parser::parsePointLight() {
    BRACE_BEGIN();
    GET_PARAMETER(position, Vector3f);
    GET_PARAMETER(color, Color);
    BRACE_END();
    pScene->addLight(new PointLight(position, color));
}

void Parser::parseMaterials() {
    std::string token;
    BRACE_BEGIN();
    GET_PARAMETER(numMaterials, int);
    for (int i = 0; i < numMaterials; i++) {
        getToken(token);
        if (token == "PhongMaterial")
            parsePhongMaterial();
        else if (token == "DiffMaterial")
            parseDiffMaterial();
        else THROW_SYNTAX_ERROR(__FILE__, __LINE__);
    }
    BRACE_END();
}

void Parser::parsePhongMaterial() {
    BRACE_BEGIN();
    GET_PARAMETER(diffuseColor, Color);
    GET_PARAMETER(specularColor, Color);
    GET_PARAMETER(shininess, int);
    BRACE_END();
    pScene->addMaterial(new PhongMaterial(diffuseColor, specularColor, shininess));
}

void Parser::parseDiffMaterial() {
    BRACE_BEGIN();
    GET_PARAMETER(diffuseColor, Color);
    BRACE_END();
    pScene->addMaterial(new DiffMaterial(diffuseColor));
}

void Parser::parseTopGroup() {
    pScene->registerGroup(parseGroup());
}

Group* Parser::parseGroup() {
    BRACE_BEGIN();
    Material* currentMaterial = nullptr;
    std::string token;
    std::vector<Entity*> entities;
    GET_PARAMETER(numObjects, int);
    for (int i = 0; i < numObjects; i++) {
        getToken(token);
        if (token == "MaterialIndex")
            --i, currentMaterial = parseMaterialFromIndex();
        else if (token == "Group")
            entities.emplace_back(parseGroup());
        else
            entities.emplace_back(parseEntity(token, currentMaterial));
    }
    BRACE_END();
    return new Group(entities);
}

Material* Parser::parseMaterialFromIndex() {
    int index; stream >> index;
    return pScene->getMaterial(index);
}

Entity* Parser::parseEntity(const std::string& token, Material* currentMaterial) {
        if (token == "Sphere")
            return parseSphere(currentMaterial);
        else if (token == "Plane")
            return parsePlane(currentMaterial);
        else if (token == "Triangle")
            return parseTriangle(currentMaterial);
        else if (token == "TriangleMesh")
            return parseMesh(currentMaterial);
        else if (token == "Transform")
            return parseTransform(currentMaterial);
        else
            THROW_SYNTAX_ERROR(__FILE__, __LINE__);
}


Sphere* Parser::parseSphere(Material* material) {
    BRACE_BEGIN();
    GET_PARAMETER(center, Vector3f);
    GET_PARAMETER(radius, float);
    BRACE_END();
    return new Sphere(center, radius, material);
}

Plane* Parser::parsePlane(Material* material) {
    BRACE_BEGIN();
    GET_PARAMETER(normal, Vector3f);
    GET_PARAMETER(offset, float);
    BRACE_END();
    return new Plane(normal, offset, material);
}

Triangle* Parser::parseTriangle(Material* material) {
    BRACE_BEGIN();
    GET_PARAMETER(vertex0, Vector3f);
    GET_PARAMETER(vertex1, Vector3f);
    GET_PARAMETER(vertex2, Vector3f);
    BRACE_END();
    return new Triangle(vertex0, vertex1, vertex2);
}

Mesh* Parser::parseMesh(Material* material) {
    BRACE_BEGIN();
    GET_PARAMETER(obj_file, std::string);
    BRACE_END();
    return new Mesh(obj_file.c_str(), 0, material);
}

Transform* Parser::parseTransform(Material* material) {
    Matrix4f matrix = Matrix4f::identity();
    Entity* entity = nullptr;
    std::string token;
    BRACE_BEGIN();
    while (true) {
        getToken(token);
        if (token == "Scale") {
            GET(s, Vector3f);
            matrix = matrix * Matrix4f::scaling(s[0], s[1], s[2]);
        } else if (token == "UniformScale") {
            GET(s, float);
            matrix = matrix * Matrix4f::uniformScaling(s);
        } else if (token == "Translate") {
            GET(translation, Vector3f);
            matrix = matrix * Matrix4f::translation(translation);
        } else if (token == "XRotate") {
            GET(d, float);
            matrix = matrix * Matrix4f::rotateX(DEGREE_TO_RADIANS(d));
        } else if (token == "YRotate") {
            GET(d, float);
            matrix = matrix * Matrix4f::rotateY(DEGREE_TO_RADIANS(d));
        } else if (token == "ZRotate") {
            GET(d, float);
            matrix = matrix * Matrix4f::rotateZ(DEGREE_TO_RADIANS(d));
        } else if (token == "Rotate") {
            BRACE_BEGIN();
            GET(axis, Vector3f);
            GET(degrees, float);
            matrix = matrix * Matrix4f::rotation(axis, DEGREE_TO_RADIANS(degrees));
            BRACE_END();
        } else if (token == "Matrix4f") {
            Matrix4f matrix2 = Matrix4f::identity();
            BRACE_BEGIN();
            for (int j = 0; j < 4; j++) {
                for (int i = 0; i < 4; i++) {
                    GET(v, float);
                    matrix2(i, j) = v;
                }
            }
            BRACE_END();
            matrix = matrix2 * matrix;
        } else {
            // otherwise this must be an object,
            // and there are no more transformations
            entity = parseEntity(token, material);
            break;
        }
    }
    BRACE_END();
    return new Transform(entity, matrix);
}

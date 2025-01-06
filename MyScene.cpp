#include "MyScene.h"

// Constructor
MyScene::MyScene()
    : angulo(0.0f), mainModel(nullptr), skyDome(nullptr), terreno(nullptr) {
    // Initialize models, terrain, and sky dome here if needed
}

// Destructor
MyScene::~MyScene() {
    // Clean up allocated resources if needed
}

// Implement pure virtual functions
float MyScene::getAngulo() {
    return angulo;
}

void MyScene::setAngulo(float newAngulo) {
    angulo = newAngulo;
}

Model* MyScene::getMainModel() {
    return mainModel; // Return the main model, if set
}

Scene* MyScene::Render() {
    // Render logic for the scene
    return this;
}

SkyDome* MyScene::getSky() {
    return skyDome;
}

Terreno* MyScene::getTerreno() {
    return terreno;
}

std::vector<Model*>* MyScene::getLoadedModels() {
    return &loadedModels;
}

std::vector<Billboard*>* MyScene::getLoadedBillboards() {
    return &loadedBillboards;
}
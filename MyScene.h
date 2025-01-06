#pragma once

#include "Base/Scene.h" // Include the base class

class MyScene : public Scene {
public:
    MyScene(); // Constructor declaration
    ~MyScene(); // Destructor declaration (optional)

    // Override all pure virtual functions
    float getAngulo() override;
    void setAngulo(float angulo) override;
    Model* getMainModel() override;
    Scene* Render() override;
    SkyDome* getSky() override;
    Terreno* getTerreno() override;
    std::vector<Model*>* getLoadedModels() override;
    std::vector<Billboard*>* getLoadedBillboards() override;

private:
    // Add member variables specific to MyScene
    float angulo;
    Model* mainModel;
    SkyDome* skyDome;
    Terreno* terreno;
    std::vector<Model*> loadedModels;
    std::vector<Billboard*> loadedBillboards;
};
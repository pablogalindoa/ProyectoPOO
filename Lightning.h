#pragma once
#include <glm/glm.hpp>
#include "Base/Shader.h"

class Lighting {
public:
    glm::vec3 ambientColor;
    glm::vec3 lightPosition;

    Lighting() : ambientColor(glm::vec3(1.0f, 1.0f, 1.0f)), lightPosition(glm::vec3(0.0f, 10.0f, 0.0f)) {}

    // Set ambient lighting color based on weather
    void setAmbientColor(const glm::vec3& color) {
        ambientColor = color;
    }

    // Update lighting (you can add more properties like light intensity, etc.)
    void update(Shader& shader) {
        shader.setVec3("ambientLight", ambientColor);
        shader.setVec3("lightPosition", lightPosition);
    }
};
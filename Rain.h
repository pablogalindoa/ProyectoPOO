#pragma once
#include <vector>
#include <glm/glm.hpp>

class RainSystem {
public:
    struct RainDrop {
        glm::vec3 position;  // Position of the raindrop
        float speed;         // Speed of the raindrop falling
        float size;          // Size of the raindrop (can be width/height of the line)
    };

    RainSystem(int numDrops);  // Constructor to initialize raindrops
    void updateRain(float deltaTime);  // Update raindrops' positions
    void renderRain();  // Render raindrops as small lines

private:
    std::vector<RainDrop> rainDrops;  // Vector to store the raindrops
};


#include "Rain.h"
#include <GL/glew.h>  // Or your preferred OpenGL wrapper

RainSystem::RainSystem(int numDrops) {
    // Generate the initial raindrops
    for (int i = 0; i < numDrops; i++) {
        RainDrop drop;
        drop.position = glm::vec3(rand() % 100 - 50, rand() % 100 + 50, 0); // Random positions
        drop.speed = (rand() % 5 + 2) / 10.0f; // Random speed
        drop.size = (rand() % 3 + 1) / 10.0f;  // Random size of raindrop
        rainDrops.push_back(drop);
    }
}

void RainSystem::updateRain(float deltaTime) {
    for (auto& drop : rainDrops) {
        drop.position.y -= drop.speed * deltaTime;  // Move raindrop downwards

        // Recycle the raindrop to the top once it goes off-screen
        if (drop.position.y < -50.0f) {
            drop.position.y = 50.0f; // Reset to top
            drop.position.x = rand() % 100 - 50;  // Randomize horizontal position
        }
    }
}

void RainSystem::renderRain() {
    glEnable(GL_BLEND); // Enable blending for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set blend function
    for (const auto& drop : rainDrops) {
        // Here, draw a small vertical line to represent each raindrop
        glBegin(GL_LINES);
        glColor4f(0.5f, 0.5f, 1.0f, 0.5f);  // Set color to light blue with transparency
        glVertex3f(drop.position.x, drop.position.y, 0.0f);  // Top of the raindrop
        glVertex3f(drop.position.x, drop.position.y - drop.size, 0.0f);  // Bottom of the raindrop
        glEnd();
    }
    glDisable(GL_BLEND); // Disable blending to avoid affecting other objects
}
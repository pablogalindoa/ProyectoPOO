#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Base/Utilities.h"
#include "Base/Shader.h"
//#include "Base/Particle.h"  // Assuming you have a basic particle system class

//class RainSystem {
//public:
//    struct RainDrop {
//        glm::vec3 position;
//        glm::vec3 velocity;
//    };
//
//private:
//    std::vector<RainDrop> rainDrops;
//    bool enabled = false; // Toggle rain visibility
//    float rainIntensity = 1.0f; // Intensity of the rain
//
//public:
//    RainSystem(int numParticles) {
//        // Initialize the rain system with a number of particles
//        for (int i = 0; i < numParticles; ++i) {
//            RainDrop drop;
//            drop.position = glm::vec3(rand() % 200 - 100, rand() % 100 + 50, rand() % 200 - 100); // Random positions
//            drop.velocity = glm::vec3(0.0f, -0.1f, 0.0f); // Falling downwards
//            rainDrops.push_back(drop);
//        }
//    }
//
//    // Toggle rain visibility
//    void setEnabled(bool enable) {
//        enabled = enable;
//    }
//
//    // Set the intensity of the rain (e.g., faster falling or more particles)
//    void setRainIntensity(float intensity) {
//        rainIntensity = intensity;
//    }
//
//    // Update the rain system (move particles, handle collisions, etc.)
//    void update() {
//        if (!enabled) return;
//
//        for (auto& drop : rainDrops) {
//            drop.position += drop.velocity * rainIntensity; // Move the drops down
//            if (drop.position.y < 0) {
//                // Reset raindrop position when it hits the ground
//                drop.position.y = rand() % 100 + 50;
//                drop.position.x = rand() % 200 - 100;
//                drop.position.z = rand() % 200 - 100;
//            }
//        }
//    }
//
//    // Draw the rain particles
//    void draw(Shader& shader) {
//        if (!enabled) return;
//
//        // Render each raindrop as a simple point or small quad
//        for (const auto& drop : rainDrops) {
//            shader.setMat4("model", glm::translate(glm::mat4(1.0f), drop.position));
//            // Here you would draw the particle, for example as a small quad or point
//        }
//    }
//};
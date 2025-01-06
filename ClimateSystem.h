#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "SkyDome.h"

class ClimateManager {
private:
    glm::vec3 currentLightColor;
    glm::vec3 currentAmbientColor;
    glm::vec3 currentDiffuseColor;
    float lightIntensity;

public:
    // Constructor to initialize light properties
    ClimateManager() {
        currentLightColor = glm::vec3(1.0f, 1.0f, 1.0f);  // Default sunny
        currentAmbientColor = glm::vec3(0.3f, 0.3f, 0.3f); // Mild ambient light
        currentDiffuseColor = glm::vec3(1.0f, 1.0f, 1.0f); // Full diffuse light
        lightIntensity = 1.0f;  // Normal intensity
    }

    // Update the light color and intensity to simulate weather
    void updateClimate(int climateType) {
        switch (climateType) {
        case 0: // Sunny
            currentLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
            currentAmbientColor = glm::vec3(0.3f, 0.3f, 0.3f);
            currentDiffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
            lightIntensity = 1.0f;
            break;

        case 1: // Cloudy
            currentLightColor = glm::vec3(0.7f, 0.7f, 0.7f);
            currentAmbientColor = glm::vec3(0.5f, 0.5f, 0.5f);
            currentDiffuseColor = glm::vec3(0.8f, 0.8f, 0.8f);
            lightIntensity = 0.7f;
            break;

        case 2: // Stormy
            currentLightColor = glm::vec3(0.1f, 0.1f, 0.1f);
            currentAmbientColor = glm::vec3(0.2f, 0.2f, 0.2f);
            currentDiffuseColor = glm::vec3(0.3f, 0.3f, 0.3f);
            lightIntensity = 0.4f;
            break;

        case 3: // Sunset
            currentLightColor = glm::vec3(1.0f, 0.5f, 0.2f);
            currentAmbientColor = glm::vec3(0.6f, 0.3f, 0.1f);
            currentDiffuseColor = glm::vec3(0.9f, 0.5f, 0.2f);
            lightIntensity = 0.8f;
            break;

        default:
            break;
        }
    }
    // Set the light properties in the shader
    void applyLighting(Shader& shader) {
        shader.setVec3("light.color", currentLightColor);
        shader.setVec3("light.ambient", currentAmbientColor);
        shader.setVec3("light.diffuse", currentDiffuseColor);
        shader.setFloat("light.intensity", lightIntensity);
    }
};

//class ClimateSystem {
//public:
//    enum class Weather { CLEAR, RAINY, STORMY };
//    Weather currentWeather;
//
//    SkyDome* sky;
//    RainSystem* rainSystem;
//    Lighting* lighting;
//
//    ClimateSystem(SkyDome* sky, RainSystem* rainSystem, Lighting* lighting)
//        : sky(sky), rainSystem(rainSystem), lighting(lighting) {
//        currentWeather = Weather::CLEAR; // Default to clear weather
//    }
//
//    void updateWeather() {
//        // Randomly change the weather every 5 seconds for this example
//        static float timer = 0.0f;
//        timer += 0.1f;  // Simulate time passing (adjust this rate as needed)
//        if (timer > 5.0f) {
//            changeWeather();  // Change the weather randomly every 5 seconds
//            timer = 0.0f;
//        }
//
//        applyWeatherEffects();
//    }
//
//private:
//    void changeWeather() {
//        // Randomly choose a weather condition
//        int randomWeather = rand() % 3;
//        switch (randomWeather) {
//        case 0:
//            currentWeather = Weather::CLEAR;
//            break;
//        case 1:
//            currentWeather = Weather::RAINY;
//            break;
//        case 2:
//            currentWeather = Weather::STORMY;
//            break;
//        }
//
//        // Log the current weather for debugging
//        std::string weatherStr = (currentWeather == Weather::CLEAR) ? "Clear"
//            : (currentWeather == Weather::RAINY) ? "Rainy"
//            : "Stormy";
//        std::cout << "Current Weather: " << weatherStr << std::endl;
//    }
//
//
//
//    void applyWeatherEffects() {
//        // Apply weather effects to the environment
//        switch (currentWeather) {
//        case Weather::CLEAR:
//            sky->setSkyTexture(L"skydome/clear_sky.jpg"); // Set clear sky texture
//            rainSystem->setEnabled(false);  // Disable rain effect
//            lighting->setAmbientColor(glm::vec3(1.0f, 1.0f, 1.0f)); // Normal lighting
//            break;
//
//        case Weather::RAINY:
//            sky->setSkyTexture(L"skydome/rainy_sky.jpg"); // Set rainy sky texture
//            rainSystem->setEnabled(true);  // Enable rain effect
//            lighting->setAmbientColor(glm::vec3(0.7f, 0.7f, 0.7f)); // Dim lighting for rain
//            break;
//
//        case Weather::STORMY:
//            sky->setSkyTexture(L"skydome/stormy_sky.jpg"); // Set stormy sky texture
//            rainSystem->setEnabled(true);  // Enable rain effect
//            rainSystem->setRainIntensity(2.0f);  // Increase rain intensity for storm
//            lighting->setAmbientColor(glm::vec3(0.3f, 0.3f, 0.3f)); // Darker lighting for storm
//            break;
//        }
//    }
//};
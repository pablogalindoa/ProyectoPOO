//#include "SkyDome.h"
//
//// Define the setSkyTexture method
//void SkyDome::setSkyTexture(WCHAR* texturePath) {
//    // Load the new texture
//    string textureName((const wchar_t*)texturePath);
//    unsigned int newTexture = TextureFromFile(textureName.c_str(), this->directory);
//
//    // Replace the existing texture
//    textures_loaded.clear();  // Clear any previously loaded textures
//    Texture newTextureObj = { newTexture, "texture_diffuse", textureName.c_str() };
//    textures_loaded.push_back(newTextureObj);
//    meshes[0].textures.clear();  // Clear any textures already assigned to meshes
//    meshes[0].textures.push_back(newTextureObj);  // Assign the new texture
//}
//
//// Function to load texture from file (if not already implemented)
//unsigned int SkyDome::TextureFromFile(const char* path, const string& directory) {
//    string filename = string(path);
//    filename = directory + '\\' + filename;
//
//    unsigned int textureID;
//    glGenTextures(1, &textureID);
//    glBindTexture(GL_TEXTURE_2D, textureID);
//
//    // Load texture
//    int width, height, nrChannels;
//    stbi_set_flip_vertically_on_load(true); // Optional: flip texture vertically
//    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
//    if (data) {
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//    }
//    else {
//        std::cout << "Texture failed to load at path: " << path << std::endl;
//    }
//    stbi_image_free(data);
//
//    return textureID;
//}
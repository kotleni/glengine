#ifndef H_RESOURCES_MANAGER
#define H_RESOURCES_MANAGER

#include <map>
#include <string>
#include <functional> 
#include <dirent.h> 
#include <filesystem>
#include "shader.hpp"
#include "model.hpp"

#define SHADERS_FOLDER "../assets/shaders/"
#define MODELS_FOLDER "../assets/models/"
//#define IMAGES_FOLDER "../assets/images/"

class ResourcesManager {
private:
    std::map<std::string, Shader*> shaders;
    std::map<std::string, Model*> models;
    // std::map<std::string, unsigned int> images;
    void loadShaders();
    void loadModels();
    //void loadImages();
public:
    ResourcesManager() {}

    void loadAll();
    Shader *getShader(std::string key);
    Model *getModel(std::string key);
    // unsinged int getImage(std::string key);
};

#endif
#include "resources_manager.hpp"

// TODO: move to utils
void listFiles(const std::string path, std::function<void(const std::string)> cb) {
    if (auto dir = opendir(path.c_str())) {
        while (auto f = readdir(dir)) {
            if (!f->d_name || f->d_name[0] == '.') continue;
            if (f->d_type == DT_DIR) listFiles(path + f->d_name + "/", cb);
            if (f->d_type == DT_REG) cb(path + f->d_name); 
        } 
        
        closedir(dir); 
    }
}

// Shader* loadShader(std::string path);
void ResourcesManager::loadShaders() {
    listFiles(std::string(SHADERS_FOLDER), [this](const std::string path) {
        if(path.ends_with(".vert")) { // Prevent dublicates
            std::filesystem::path filePath(path);
            std::string fileName = filePath.stem().string();
            printf("[ResourcesManager] Loading shader %s\n", fileName.c_str());
            this->shaders[fileName] = Shader::load(fileName);
        }
    });
}

void ResourcesManager::loadModels() {
    listFiles(std::string(MODELS_FOLDER), [this](const std::string path) {
        if(path.ends_with(".obj")) { // Allowed only obj models
            std::string fullPath = path;
            printf("[ResourcesManager] Loading model %s\n", fullPath.c_str());
            this->models[fullPath] = new Model(fullPath);
        }
    });
}

void ResourcesManager::loadAll() {
    this->loadShaders();
    this->loadModels();
}

Shader *ResourcesManager::getShader(std::string key) {
    return this->shaders[key];
}

Model *ResourcesManager::getModel(std::string key) {
    return this->models[key];
}
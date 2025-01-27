#pragma once

#include "Module.h"
#include <string>

class ModuleSkybox : public Module {

public:

    ModuleSkybox();
    ~ModuleSkybox();

    bool Init() override;
    update_status PreUpdate() override;
    update_status Update() override;
    update_status PostUpdate() override;
    bool CleanUp() override;

    void RenderSkybox();


private:

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    unsigned int cubemapTexture;
    unsigned int skyboxProgram = 0;


};
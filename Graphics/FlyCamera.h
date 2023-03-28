#pragma once
#include "BaseCamera.h"

class FlyCamera :
    public BaseCamera
{
public:
    FlyCamera();
    FlyCamera(glm::vec3 pos, float theta, float phi);
    ~FlyCamera();

    void update(float deltaTime) override;
};


#pragma once
#include "BaseCamera.h"

class FlyCamera :
    public BaseCamera
{
public:
    FlyCamera();
    ~FlyCamera();

    void update(float deltaTime) override;
};


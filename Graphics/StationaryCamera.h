#pragma once
#include "BaseCamera.h"
class StationaryCamera :
    public BaseCamera
{
public:
    StationaryCamera();
    ~StationaryCamera();

    void update(float deltaTime) override;
};
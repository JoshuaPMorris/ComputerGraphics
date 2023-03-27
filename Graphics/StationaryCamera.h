#pragma once
#include "BaseCamera.h"
class StationaryCamera :
    public BaseCamera
{
public:
    StationaryCamera();
    StationaryCamera(bool xAxis, bool yAxis, bool zAxis);
    ~StationaryCamera();

    void update(float deltaTime) override;

private:
    bool m_xAxis = false;
    bool m_yAxis = false;
    bool m_zAxis = false;
};
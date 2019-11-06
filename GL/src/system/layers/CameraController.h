#pragma once

#include "../../Common.h"
#include "../Layer.h"

#include "../../Graphics.h"

class CameraController : public Layer
{
private:
    Camera camera;

public:
    virtual void onLoad() override;
    virtual void onUnload() override;
};
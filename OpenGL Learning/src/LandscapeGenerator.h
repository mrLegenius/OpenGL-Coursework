#pragma once
#include <memory>
#include "Shape3D.h"
#include "Texture.h"
class LandscapeGenerator
{
private:
    static float map(float value, float min1, float max1, float min2, float max2);
public:
    static std::shared_ptr<Shape3D> Generate(std::shared_ptr<Texture> heightMap, float heightMultiplier = 1);
};


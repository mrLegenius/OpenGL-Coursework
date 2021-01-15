#pragma once
#include <memory>
#include "Shape3D.h"
#include "Texture.h"
class LandscapeGenerator
{
private:

public:
    static std::shared_ptr<Shape3D> Generate(GLuint dimensions, float amplitude = 1, int frequency = 1);
};


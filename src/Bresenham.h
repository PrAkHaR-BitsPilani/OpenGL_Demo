#pragma once
#include <vector>
#include "external/glm/glm.hpp"

#define delta 0.001f
#define invDelta 1000
#define invDeltaSQ 1000000

namespace Bresenham
{
    std::vector<float> drawAxis(int max);
    std::vector<float> drawLine(glm::vec3 p1 , glm::vec3 p2);
    std::vector<float> drawCircle(glm::vec3 centre , int radius);
}

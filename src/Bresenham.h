#pragma once
#include <utility>
#include <vector>
#include "external/glm/glm.hpp"

#define delta 0.001f
#define invDelta 1000
#define invDeltaSQ 1000000

namespace Bresenham
{
    std::vector<float> drawAxis(int max);
    std::vector<float> drawLine(std::pair<int,int>p1 , std::pair<int,int> p2);
    std::vector<float> drawCircle(glm::vec3 centre , int radius);
}

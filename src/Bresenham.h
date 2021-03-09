#pragma once
#include <vector>
#include <utility>
#include "external/glm/glm.hpp"

#define delta 0.01f

namespace Bresenham
{
    std::vector<float> drawAxis(int max , glm::vec3 color);
    std::vector<float> drawLine(glm::vec3 p1 , glm::vec3 p2 , glm::vec3 color);
    std::vector<float> drawCircle(glm::vec3 centre , float radius , glm::vec3 color);
}

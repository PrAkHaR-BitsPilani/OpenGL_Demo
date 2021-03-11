#pragma once
#include <vector>
#include <utility>
#include "external/glm/glm.hpp"

#define delta 0.01f
///Basic drawing functions
namespace Bresenham
{
    ///Calculates the coordinates for the X,Y,Z axes
    ///@param max denotes the range for X,Y,Z.
    ///@param color denotes the color of the axes
    ///@return a std::vector<float> containing the vertices for the X,Y,Z axes
    std::vector<float> drawAxis(int max , std::vector<float> color);
    ///Calculates the coordinates or a line.
    ///@param p1 denotes the starting point
    ///@param p2 denotes the ending point
    ///@param color denotes the color of the line
    ///@return std::vector<float> containing the vertices for the line joining \p p1 and \p p2
    ///@note the vertices returned between \p p1 and \p p2 might not be the same as that between \p p2 and \p p1
    std::vector<float> drawLine(glm::vec3 p1 , glm::vec3 p2 , std::vector<float> color);
    ///Calculates the coordinates of a circle.
    ///@param centre the centre of the circle.
    ///@param radius the radius of the circle.
    ///@param color the color of the circle.
    ///@return std::vector<float> containing the vertices of the circle
    ///@attention There is a chance of observing 4 jumps  at 4 symmetric points on the circle. This is because 8-fold symmetry is used in the implementation.
    std::vector<float> drawCircle(glm::vec3 centre , float radius , std::vector<float> color);
}


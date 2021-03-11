#include "Bresenham.h"
#include <iostream>

void addCirclePoints(std::vector<float>& ans , float x , float y , float z , glm::vec3 centre , std::vector<float> color);

float abs(float f)
{
    if(f < 0.0f)return -f;
    return f;
}

std::vector<float> Bresenham::drawLine(glm::vec3 p1 , glm::vec3 p2 , std::vector<float> color)
{  

    /*
        Standadrd equation of line is aX + bY + c = 0.
        We are given two points @param p1 , @oaram p2 which lie on  this line. This leads to the following observation : 
        a = ΔX
        b = -ΔY
        delta -> determines how much we increase X or Y in each iteration. Default value is 0.01;
    */

    std::vector<float> ans;

    assert(p1.z == p2.z);       // Currently no change in the Z Coordinate is supported :(...........I might implement Bresenham for 3D later in future.

    float delX = p2.x - p1.x;
    float delY = p2.y - p1.y;
    float a = delY;
    float b = -delX;

    int x_dir = (p1.x <= p2.x) ? 1 : -1;        //determines whether X is increasing or decreasing
    int y_dir = (p1.y <= p2.y) ? 1 : -1;        //determines whether Y is increasing or decreasing

    a *= x_dir;
    b *= y_dir;
    
    if(abs(delY) <= abs(delX))
    {                                   // Absolute value of slope is less than or equal to 1
        /*
            dir1 -> going along x_dir
            dir2 -> going along both x_dir and y_dir
            Decision variable  will help us to choose between dir1 and dir2.
        */
        float d = 2*a + b;              // initial value of the decision variable
        float inc_dir1 = 2*a;           // change in d if we go in dir1...........(along x_dir)
        float inc_dir2 = 2*(a+b);       // change in d if we go in dir2...........(along both x_dir and y_dir)

        // initial coordinates
        float x = p1.x;                 
        float y = p1.y;
        float z = p1.z;

        ans.emplace_back(x);ans.emplace_back(y);ans.emplace_back(z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);

        while((x_dir * x) <= (x_dir * p2.x))
        {
            if((x_dir * y_dir * d) <= 0){
                d += inc_dir1;          // going in dir1
            }else{
                d += inc_dir2;          // going in dir2
                y += y_dir * delta;
            }
            x += x_dir * delta;

            ans.emplace_back(x);ans.emplace_back(y);ans.emplace_back(z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
        }
    }
    else
    {                                   // Absolute value of slope is greater than 1.
        /*
            dir1 -> going along x_dir
            dir2 -> going along both x_dir and y_dir
            Decision variable  will help us to choose between dir1 and dir2.
        */
        float d = a + 2*b;              // initial value of the decision variable
        float inc_dir1 = 2*b;           // change in d if we go in dir1...........(along x_dir)
        float inc_dir2 = 2*(a+b);       // change in d if we go in dir2...........(along both x_dir and y_dir)

        // initial coordinates
        float x = p1.x;
        float y = p1.y;
        float z = p1.z;

        ans.emplace_back(x);ans.emplace_back(y);ans.emplace_back(z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);

        while((y_dir * y) <= (y_dir * p2.y))
        {
            if((x_dir * y_dir * d) > 0){
                d += inc_dir1;          // going in dir1
            }else{
                d += inc_dir2;          // goint in dir2
                x += x_dir * delta;
            }
            y += y_dir * delta;

            ans.emplace_back(x);ans.emplace_back(y);ans.emplace_back(z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
        }
    }
    return ans;
}

std::vector<float> Bresenham::drawCircle(glm::vec3 centre , float radius , std::vector<float>color)
{
    /*
    Standard oequation of circle is (x-a)^2 + (y-b)^2 = r^2
    We fill draw a circle with centre as origin (0,0,0) and then we will translate all the points.

    This versoin of the MidPoint Circle Drawing algo uses quite a bit floating point operations. I have ignored terms of order > 10^2.

    */

    std::vector<float> ans;

    // initial coordinates
    float x = 0.0f;
    float y = radius;
    float z = 0.0f;

    float d = 5*0/4 - radius*delta;                 // initial value of the decision variable

    float delE = 2*delta + 0;                       //change in d if we go in East direction
    float delSE = -2* radius + 2*delta + 3*0;       //change in d if we go in South-East direction

    addCirclePoints(ans , x , y , z , centre , color);      // inserts 8 points in the vector using the 8=fold symmetry property of a circle.

    while(y > x)
    {
        if(d < 0){                                  // going in the East direction
            d += delE;
            delE += 2*delta + 0;
            delSE += 2*delta + 0;
        }else{
            d += delSE;                             // going in the South-East direction
            delE += 2*delta + 0;
            delSE += 2*delta + 3*0;
            y -= delta;
        }
        x += delta;

        addCirclePoints(ans , x , y , z , centre , color);          // inserts 8 points in the vector using the 8=fold symmetry property of a circle.
    }

    return ans;
}

std::vector<float> Bresenham::drawAxis(int max , std::vector<float> color)
{
    std::vector<float> ans;
    float x = 0.0f;
    while(x <= max)
    {
        ans.emplace_back(x);ans.emplace_back(0.0f);ans.emplace_back(0.0f);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);         //(x,0,0) (color)
        ans.emplace_back(0.0f);ans.emplace_back(x);ans.emplace_back(0.0f);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);         //(0,x,0) (color)
        ans.emplace_back(0.0f);ans.emplace_back(0.0f);ans.emplace_back(x);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);         //(0,0,x) (color)
        ans.emplace_back(-x);ans.emplace_back(0.0f);ans.emplace_back(0.0f);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);        //(-x,0,0) (color)
        ans.emplace_back(0.0f);ans.emplace_back(-x);ans.emplace_back(0.0f);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);        //(0,-x,0) (color)
        ans.emplace_back(0.0f);ans.emplace_back(0.0f);ans.emplace_back(-x);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);        //(0,0,-x) (color)
        x += delta;
    }
    return ans;
}

void addCirclePoints(std::vector<float>& ans , float x , float y , float z , glm::vec3 centre , std::vector<float> color){

    /*
    inserts the following points
    ( x, y, z) (color)
    ( x,-y, z) (color)
    (-x, y, z) (color)
    (-x,-y, z) (color)
    ( y, x, z) (color)
    ( y,-x, z) (color)
    (-y, x, z) (color)
    (-y,-x, z) (color)
    */

    ans.emplace_back(centre.x + x);ans.emplace_back(centre.y + y);ans.emplace_back(centre.z + z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
    ans.emplace_back(centre.x + x);ans.emplace_back(centre.y + -y);ans.emplace_back(centre.z + z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
    ans. emplace_back(centre.x + -x);ans.emplace_back(centre.y + y);ans.emplace_back(centre.z + z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
    ans.emplace_back(centre.x + -x);ans.emplace_back(centre.y + -y);ans.emplace_back(centre.z + z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);

    ans.emplace_back(centre.x + y);ans.emplace_back(centre.y + x);ans.emplace_back(centre.z + z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
    ans.emplace_back(centre.x + y);ans.emplace_back(centre.y + -x);ans.emplace_back(centre.z + z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
    ans.emplace_back(centre.x + -y);ans.emplace_back(centre.y + x);ans.emplace_back(centre.z + z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
    ans.emplace_back(centre.x + -y);ans.emplace_back(centre.y + -x);ans.emplace_back(centre.z + z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);

}

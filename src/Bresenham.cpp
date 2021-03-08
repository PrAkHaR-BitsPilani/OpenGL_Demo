#include "Bresenham.h"
#include <iostream>

void addCirclePoints(std::vector<float>& ans , float x , float y , float z , glm::vec3 centre);

std::vector<float> Bresenham::drawLine(glm::vec3 p1 , glm::vec3 p2)
{  

    assert(p1.z == p2.z);

    std::vector<float> ans;
    bool swapFlag = false;
    if(p1.x > p2.x){
        glm::vec3 temp = p1;
        p1 = p2;
        p2 = temp;
        swapFlag = true;
    }
    int delX = p2.x - p1.x;
    int delY = p2.y - p1.y;
    int a = delY;
    int b = -delX;
    if(abs(delY) <= abs(delX))
    {
        
        int y_dir = (p1.y <= p2.y) ? 1 : -1;
        b *= y_dir;

        int d = 2*a + b;
        int incE = 2*a;
        int incNE = 2*(a+b);

        float x = p1.x;
        float y = p1.y;
        float z = p1.z;

        ans.emplace_back(x);
        ans.emplace_back(y);
        ans.emplace_back(z);

        while(x <= p2.x)
        {
            if((y_dir * d) <= 0){
                d += incE;
            }else{
                d += incNE;
                y += y_dir * delta;
            }
            x += delta;

            ans.emplace_back(x);
            ans.emplace_back(y);
            ans.emplace_back(z);
        }
    }
    else
    {
        int y_dir = p1.y <= p2.y ? 1 : -1;
        b *= y_dir;

        int d = a + 2*b;
        int incN = 2*b;
        int incNE = 2*(a+b);

        float x = p1.x;
        float y = p1.y;
        float z = p1.z;

        ans.emplace_back(x);
        ans.emplace_back(y);
        ans.emplace_back(z);

        while((y_dir * y) <= (y_dir * p2.y))
        {
            if((y_dir * d) > 0){
                d += incN;
            }else{
                d += incNE;
                x += delta;
            }
            y += y_dir * delta;

            ans.emplace_back(x);
            ans.emplace_back(y);
            ans.emplace_back(z);
        }
    }
    return ans;
}

std::vector<float> Bresenham::drawCircle(glm::vec3 centre , int radius)
{
    std::vector<float> ans;

    float x = 0.0f;
    float y = radius;
    float z = 0.0f;
    float d = 5*0/4 - radius*delta;

    float delE = 2*delta + 0;
    float delSE = -2* radius + 2*delta + 3*0;

    addCirclePoints(ans , x , y , z , centre);

    while(y > x)
    {
        if(d < 0){
            d += delE;
            delE += 2*delta + delta *delta;
            delSE += 2*delta + delta *delta;
        }else{
            d += delSE;
            delE += 2*delta + 0;
            delSE += 2*delta + 3*0;
            y -= delta;
        }
        x += delta;

        addCirclePoints(ans , x , y , z , centre);
    }

    return ans;
}

std::vector<float> Bresenham::drawAxis(int max)
{
    std::vector<float> ans;
    float x = 0.0f;
    while(x <= max)
    {
        ans.emplace_back(x);
        ans.emplace_back(0.0f);
        ans.emplace_back(0.0f);
        ans.emplace_back(0.0f);
        ans.emplace_back(x);
        ans.emplace_back(0.0f);
        ans.emplace_back(0.0f);
        ans.emplace_back(0.0f);
        ans.emplace_back(x);
        ans.emplace_back(-x);
        ans.emplace_back(0.0f);
        ans.emplace_back(0.0f);
        ans.emplace_back(0.0f);
        ans.emplace_back(-x);
        ans.emplace_back(0.0f);
        ans.emplace_back(0.0f);
        ans.emplace_back(0.0f);
        ans.emplace_back(-x);
        x += delta;
    }
    return ans;
}

void addCirclePoints(std::vector<float>& ans , float x , float y , float z , glm::vec3 centre){
    ans.emplace_back(centre.x + x);ans.emplace_back(centre.y + y);ans.emplace_back(centre.z + z);
    ans.emplace_back(centre.x + x);ans.emplace_back(centre.y + -y);ans.emplace_back(centre.z + z);
    ans. emplace_back(centre.x + -x);ans.emplace_back(centre.y + y);ans.emplace_back(centre.z + z);
    ans.emplace_back(centre.x + -x);ans.emplace_back(centre.y + -y);ans.emplace_back(centre.z + z);

    ans.emplace_back(centre.x + y);ans.emplace_back(centre.y + x);ans.emplace_back(centre.z + z);
    ans.emplace_back(centre.x + y);ans.emplace_back(centre.y + -x);ans.emplace_back(centre.z + z);
    ans.emplace_back(centre.x + -y);ans.emplace_back(centre.y + x);ans.emplace_back(centre.z + z);
    ans.emplace_back(centre.x + -y);ans.emplace_back(centre.y + -x);ans.emplace_back(centre.z + z);

}
#include "Bresenham.h"
#include <iostream>

void addCirclePoints(std::vector<float>& ans , float x , float y , float z , glm::vec3 centre);

std::vector<float> Bresenham::drawLine(std::pair<int,int>p1 , std::pair<int,int> p2)
{  
    std::vector<float> ans;
    bool swapFlag = false;
    if(p1.first > p2.first)
            swap(p1 , p2) , swapFlag = true;
    int delX = p2.first - p1.first;
    int delY = p2.second - p1.second;
    int a = delY;
    int b = -delX;
    if(abs(delY) <= abs(delX))
    {
        
        int y_dir = (p1.second <= p2.second) ? 1 : -1;
        b *= y_dir;

        int d = 2*a + b;
        int incE = 2*a;
        int incNE = 2*(a+b);

        float x = p1.first;
        float y = p1.second;
        float z = 0.0f;

        ans.emplace_back(x);
        ans.emplace_back(y);
        ans.emplace_back(z);

        while(x <= p2.first)
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
        int y_dir = p1.second <= p2.second ? 1 : -1;
        b *= y_dir;

        int d = a + 2*b;
        int incN = 2*b;
        int incNE = 2*(a+b);

        float x = p1.first;
        float y = p1.second;
        float z = 0.0f;

        ans.emplace_back(x);
        ans.emplace_back(y);
        ans.emplace_back(z);

        while((y_dir * y) <= (y_dir * p2.second))
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
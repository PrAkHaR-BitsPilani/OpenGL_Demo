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
    std::vector<float> ans;

    assert(p1.z == p2.z);

    float delX = p2.x - p1.x;
    float delY = p2.y - p1.y;
    float a = delY;
    float b = -delX;
    if(abs(delY) <= abs(delX))
    {
        //std::cout << "Slop less than 1\n";
        
        int x_dir = (p1.x <= p2.x) ? 1 : -1;
        int y_dir = (p1.y <= p2.y) ? 1 : -1;

        a *= x_dir;
        b *= y_dir;

        float d = 2*a + b;
        float incE = 2*a;
        float incNE = 2*(a+b);

        float x = p1.x;
        float y = p1.y;
        float z = p1.z;

        ans.emplace_back(x);ans.emplace_back(y);ans.emplace_back(z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);

        while((x_dir * x) <= (x_dir * p2.x))
        {
            if((x_dir * y_dir * d) <= 0){
                d += incE;
            }else{
                d += incNE;
                y += y_dir * delta;
            }
            x += x_dir * delta;

            ans.emplace_back(x);ans.emplace_back(y);ans.emplace_back(z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
        }
    }
    else
    {

        //std::cout << "Slop greater than 1\n";

        int x_dir = (p1.x <= p2.x) ? 1 : -1;
        int y_dir = (p1.y <= p2.y) ? 1 : -1;

        a *= x_dir;
        b *= y_dir;

        float d = a + 2*b;
        float incN = 2*b;
        float incNE = 2*(a+b);

        float x = p1.x;
        float y = p1.y;
        float z = p1.z;

        ans.emplace_back(x);ans.emplace_back(y);ans.emplace_back(z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);

        while((y_dir * y) <= (y_dir * p2.y))
        {
            if((x_dir * y_dir * d) > 0){
                d += incN;
            }else{
                d += incNE;
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
    std::vector<float> ans;

    float x = 0.0f;
    float y = radius;
    float z = 0.0f;
    float d = 5*0/4 - radius*delta;

    float delE = 2*delta + 0;
    float delSE = -2* radius + 2*delta + 3*0;

    addCirclePoints(ans , x , y , z , centre , color);

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

        addCirclePoints(ans , x , y , z , centre , color);
    }

    return ans;
}

std::vector<float> Bresenham::drawAxis(int max , std::vector<float> color)
{
    std::vector<float> ans;
    float x = 0.0f;
    while(x <= max)
    {
        ans.emplace_back(x);ans.emplace_back(0.0f);ans.emplace_back(0.0f);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
        ans.emplace_back(0.0f);ans.emplace_back(x);ans.emplace_back(0.0f);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
        ans.emplace_back(0.0f);ans.emplace_back(0.0f);ans.emplace_back(x);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
        ans.emplace_back(-x);ans.emplace_back(0.0f);ans.emplace_back(0.0f);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
        ans.emplace_back(0.0f);ans.emplace_back(-x);ans.emplace_back(0.0f);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
        ans.emplace_back(0.0f);ans.emplace_back(0.0f);ans.emplace_back(-x);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
        x += delta;
    }
    return ans;
}

void addCirclePoints(std::vector<float>& ans , float x , float y , float z , glm::vec3 centre , std::vector<float> color){
    ans.emplace_back(centre.x + x);ans.emplace_back(centre.y + y);ans.emplace_back(centre.z + z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
    ans.emplace_back(centre.x + x);ans.emplace_back(centre.y + -y);ans.emplace_back(centre.z + z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
    ans. emplace_back(centre.x + -x);ans.emplace_back(centre.y + y);ans.emplace_back(centre.z + z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
    ans.emplace_back(centre.x + -x);ans.emplace_back(centre.y + -y);ans.emplace_back(centre.z + z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);

    ans.emplace_back(centre.x + y);ans.emplace_back(centre.y + x);ans.emplace_back(centre.z + z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
    ans.emplace_back(centre.x + y);ans.emplace_back(centre.y + -x);ans.emplace_back(centre.z + z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
    ans.emplace_back(centre.x + -y);ans.emplace_back(centre.y + x);ans.emplace_back(centre.z + z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);
    ans.emplace_back(centre.x + -y);ans.emplace_back(centre.y + -x);ans.emplace_back(centre.z + z);ans.emplace_back(color[0]);ans.emplace_back(color[1]);ans.emplace_back(color[2]);

}
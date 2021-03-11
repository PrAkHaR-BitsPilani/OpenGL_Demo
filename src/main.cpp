#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Bresenham.h"

#include "external/glm/glm.hpp"
#include "external/glm/gtc/matrix_transform.hpp"

#include "external/imgui/imgui.h"
#include "external/imgui/imgui_impl_glfw.h"
#include "external/imgui/imgui_impl_opengl3.h"

using namespace std;

void processInput(GLFWwindow *window);
void mouseCallBack(GLFWwindow* window, double xpos, double ypos);
void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset);

void updatePixelData();
void showDebugWindow();
vector<float>getArrowPixels(glm::vec3 endPoint , glm::vec3 vec_dir , vector<float>color);

glm::vec3 getVector(float x , float y , float z);

// window settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

// camera settings
glm::vec3 cameraPos(0.0f , 0.0f , 6.0f);
Camera camera(cameraPos);
float mouseLastX = SCR_WIDTH / 2.0;
float mouseLastY = SCR_HEIGHT / 2.0;
bool firstMouse = true;
bool locked = true;

//frame timing
float deltaTime = 0.0f;

//DEBUGGING VARIABLES
vector<float>pixels;
glm::mat4 rotA(1.0f);
glm::mat4 rotC(1.0f);
int axis_limit = 10;
vector<float> axisColor = {1.0f , 1.0f ,1.0f};
vector<float> bgColor = {0.25f , 0.25f , 0.25f,1.0f};

float camSpeed = 2.5f;

bool normalize = true;
int mode = 0;

int polyline_count = 10;
glm::vec3 polyline_start(1.0f , 1.0f , 0.0f);
int x_range = 5;
int y_range = 5;
vector<float> lineColor = {0.96f , 0.91f , 0.11f};
vector<float> circleColor = {0.11f , 0.91f , 0.96f};
float z = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Prakhar6969", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    // glad: load all OpenGL function pointers
    // ---------------------------------------

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    cout << glGetString(GL_VERSION) << "\n";

    /*
        We create a perspective projective matrix
        Field of view is returned from the camera.
        Aspect ration is set to 16:9.
        The near and far planes are set to be Z = 0.1f and Z = 100.0f
    */
    glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()) , SCR_WIDTH*1.0f/SCR_HEIGHT , 0.1f , 100.0f);

    /*
        We get the view matrix from the camera.
        This matrix corresponds to the inital state of the camera where it is placed at (0,0,6) facing at the origin.
    */
    glm::mat4 view = camera.getViewMatrix();

    Shader* shader = new Shader("res/shaders/Basic.shader");

    // We create a VertexArray object which will later be used to combine our buffer and our layout.
    VertexArray* VAO = new VertexArray();

    /*
        The layout in which the data will be stored in pur buffer will be like this -
        (posX , posY , posZ) , (colorR , colorG , colorB)
        We specifiy this by first pushing 3 floats for position and then again push 3 floats for color.
    */
    VertexBufferLayout* Layout = new VertexBufferLayout();
    Layout -> push<float>(3);
    Layout -> push<float>(3);

    /*
    Here we initialize the VertexBuffer with an empty array.
    This pixel array will change during runtime when the user makes some changes in the ImGUI and clicks on 'Apply Changes'.
    */
    VertexBuffer* VBO = new VertexBuffer(pixels.data() , pixels.size() * sizeof(float));

    // Our buffer is now linked to out VertexArray object.
    VAO -> addBuffer(*VBO , *Layout);

    /*
        We create an instance of the Renderer class.
        This will handle all our draw calls.
    */
    Renderer renderer;

    // GLCall(glEnable(GL_BLEND));
    // GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_DEPTH_TEST));

    //Setup IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window, true);    
    ImGui_ImplOpenGL3_Init("#version 330");  

    /*
        We create two rotation matrices which will rotate a vector by 30deg about the Z axis.
        rotA will rotate anti-clockwise.
        rotC will rotate clockwise.
        These matricis will be used later to create arroe heads for vector and axes.
    */
    rotA = glm::rotate(rotA , glm::radians(-30.0f) , glm::vec3(0.0f , 0.0f , 1.0f));
    rotC = glm::rotate(rotC , glm::radians(30.0f) , glm::vec3(0.0f , 0.0f , 1.0f));

    // We bind our shader here only because we are not changing the shader later.
    shader -> bind();

    // render loop  
    // -----------
    while (!glfwWindowShouldClose(window))
    {   
        processInput(window);

        // We set the bgColor
        GLCall(glClearColor(bgColor[0] , bgColor[1] , bgColor[2] , bgColor[3]));
        renderer.clear();   

        // Recalculating the projection matrix with the new FIELD OF VIEW.
        proj = glm::perspective(glm::radians(camera.getFOV()) , SCR_WIDTH*1.0f/SCR_HEIGHT , 0.1f , 100.0f);

        // setting the speed of the camera
        camera.setSpeed(camSpeed);

        // getting the view matrix corresponding to the current state of the camera.
        view = camera.getViewMatrix();

        // cout << "Projection matrix.......\n";
        // glm::mat4 sample = proj * view;
        // for(int i = 0 ; i < 4 ; i++)
        // {
        //     for(int j = 0 ; j < 4; j++)
        //         printf("%.18f " , sample[i][j]);
        //     cout << "\n";
        // }

        // IMGUI NEW FRAME
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /*
            We set the MVP matrix in our shader.
            In ouw case the Model (M) matrix is an identity matrix therefore there is no need to mention it.
        */
        shader -> setUniformMat4f("u_MVP" , proj * view);

        // Make the draw call
        renderer.drawPoints(*VAO , *shader , pixels.size());

        // We show our debug window here.
        // This debug window is for Runtime Debugging where we can adjust various parameters and analyze the vector field.
        showDebugWindow();
        ImGui::NewLine();
        ImGui::SameLine(200);
        if(ImGui::Button("Apply Changes"))
        {
            // We upadate the pixel data to what is set by the user.
            updatePixelData();
            /*
                We know update the data inside our VertexBuffer.
                NOTE : Updating the data inside our VertexBuffer does not unbinds the buffer nor does it require the bind function to be called again.
            */
            VBO->updateData(pixels.data() , pixels.size() * sizeof(float));
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        //Render IMGUI
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Shutdown IMGUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

/*
    This is a event handler for keyboard inputs.
    [WASD] is for camera movements.
    Press M to LOCK/FREE mouse.
    If mouse is in a locked state, the cursur becomes visible and only [WASD] is recorded. Changes in YAW and PITCH are ignored.
    Pressing Esc terminates the application.
*/
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // set camera speed acc to frame refresh rate
    deltaTime = glfwGetTime() - deltaTime;
    float currentTime = glfwGetTime(); 

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboardInput(camera_movements::FORWARD , deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboardInput(camera_movements::BACKWARD , deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboardInput(camera_movements::LEFT , deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboardInput(camera_movements::RIGHT , deltaTime);
    deltaTime = currentTime;

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        if(locked)
        {
            firstMouse = true;
            glfwSetInputMode(window , GLFW_CURSOR , GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(window , mouseCallBack); // for mouse movements
            glfwSetScrollCallback(window , scrollCallBack); // for mouse scrolls
            locked = false;
        }
        else
        {
            glfwSetInputMode(window , GLFW_CURSOR , GLFW_CURSOR_NORMAL);
            glfwSetCursorPosCallback(window , NULL); // for mouse movements
            glfwSetScrollCallback(window , NULL); // for mouse scrolls   
            locked = true;
        }
    }

}

/*
    This function handles changes in YAW and PITCH of the camera.
    This function records the changes in the cursor position.
    Moving mouse in the forward/backward direction changes the PITCH.
    Moving mouse in the  left/right direction changes the YAW.
*/
void mouseCallBack(GLFWwindow* window , double xpos , double ypos)
{
    if (firstMouse)
    {
        mouseLastX = xpos;
        mouseLastY = ypos;
        firstMouse = false;
    }

    float x_offset = xpos - mouseLastX;
    float y_offset = mouseLastY - ypos;
    mouseLastX = xpos;
    mouseLastY = ypos;
    camera.processMouseMovement(x_offset , y_offset);
}

/*
    This function handles the ZOOM feature by keeping track of the scroll wheel movememnts.
    Scrolling Up ZOOMS IN which is equivalent to DECREASING the FOV.
    Scrolling Down ZOOMS OUT which is equivalent to INCREASING the FOV.
*/
void scrollCallBack(GLFWwindow* window, double x_offset, double y_offset)
{
    camera.processMouseScroll(y_offset);
}

/*
    Displays the ImGUI Debug Window.
    The user can change some basic parameters of the application to see difference results.
*/
void showDebugWindow()
{
    ImGui::Text("prakhar says hello. #6969");
    ImGui::Spacing();
    if(ImGui::CollapsingHeader("Help"))
    {
        ImGui::Text("Still Under Construction :/");
    }
    if(ImGui::CollapsingHeader("Configuration"))
    {
        if(ImGui::TreeNode("Grid"))
        {
            ImGui::DragInt("Axis limit" , &axis_limit , 0.5f , 0 , 100 , "%d");
            ImGui::ColorEdit3("Axis Color" , axisColor.data());
            ImGui::ColorEdit4("Background Color" , bgColor.data());
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Camera"))
        {
            ImGui::DragFloat("Camera Speed" , &camSpeed , 0.25f , 1.0f , 5.0f);
            ImGui::NewLine();
            ImGui::SameLine(200);
            if(ImGui::Button("Recenter Camera"))
                camera.recenter(cameraPos);
            ImGui::TreePop();
        }
    }
    ImGui::Spacing();
    ImGui::ShowStyleSelector("Theme");
    ImGui::Spacing();
    
    ImGui::Checkbox("Normalize" , &normalize);
    
    
    ImGui::RadioButton("Line", &mode, 0); ImGui::SameLine();
    ImGui::RadioButton("Circle", &mode, 1); ImGui::SameLine();
    ImGui::RadioButton("Line+Circle", &mode, 2); ImGui::SameLine();
    ImGui::RadioButton("PolyLine", &mode,    3);

    ImGui::DragInt("X Range" , &x_range , 1.0f , 0 , 20 , "%d");
    ImGui::DragInt("Y Range" , &y_range , 1.0f , 0 , 20 , "%d");
    
    ImGui::ColorEdit3("Line Color" , lineColor.data());
    ImGui::ColorEdit3("Circle Color" , circleColor.data());

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Polyline count:");
    ImGui::SameLine();
    float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
    ImGui::PushButtonRepeat(true);
    if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { polyline_count--; polyline_count = max(0 , polyline_count); }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { polyline_count++; }
    ImGui::PopButtonRepeat();
    ImGui::SameLine();
    ImGui::Text("%d", polyline_count);

    ImGui::DragFloat("Polyline x0" , &polyline_start.x,0.5f,0,axis_limit);
    ImGui::DragFloat("Polyline y0" , &polyline_start.y,0.5f,0,axis_limit);

}

/*
    Updates the data which is to be rendered on the Screen.
*/
void updatePixelData()
{
    vector<float>temp;
    vector<float>arrowPixels;

    pixels = Bresenham::drawAxis(axis_limit , axisColor);           //return the coordinates for drawing the axes.

    switch(mode)
    {
        case 0:                                                     // LINE ONLY MODE
            for(int x = -x_range ; x <= x_range ; x++)              // x varies from -x_range to +x_range
            {
                for(int y  = -y_range ; y <= y_range ;y++)          // y varies from -y_range to +Y_range
                {
                    glm::vec3 vector = getVector(x,y,z);                                            // The value of the vector field at (x,y,z)
                    if(vector != glm::vec3(0.0f) && normalize)vector = glm::normalize(vector);
                    glm::vec3 endPoint = vector + glm::vec3(x,y,z);                                 // The endpoint will be (x,y,z) + f(x,y,z)
                    temp = Bresenham::drawLine(glm::vec3(x,y,z) , endPoint , lineColor);            // we get the coordinates to represent the vector
                    arrowPixels = getArrowPixels(endPoint , vector , lineColor);                    // we get the coordinates for the arrowHead
                    temp.insert(temp.end() , arrowPixels.begin() , arrowPixels.end());
                    pixels.insert(pixels.end() , temp.begin() , temp.end());
                }
            }
        break;
        case 1:                                                     // CIRCLE ONLY MODE
            for(int x = -x_range ; x <= x_range ; x++)
            {
                for(int y  = -y_range ; y <= y_range ;y++)
                {
                    glm::vec3 vector = getVector(x,y,z);                                                                                // The value of the vector field at (x,y,z)
                    if(vector != glm::vec3(0.0f) && normalize)vector = glm::normalize(vector);
                    temp = Bresenham::drawCircle(glm::vec3(x,y,z) + glm::vec3(0.5f)*vector , glm::length(vector)/2 , circleColor);      // we get the coordinates for the circle
                    pixels.insert(pixels.end() , temp.begin() , temp.end());
                }
            }
        break;
        case 2:                                                     // LINE + CIRCLE MODE
            for(int x = -x_range ; x <= x_range ; x++)
            {
                for(int y  = -y_range ; y <= y_range ;y++)
                {
                    glm::vec3 vector = getVector(x,y,z);                                                        // The value of the vector field at (x,y,z)
                    if(vector != glm::vec3(0.0f) && normalize)vector = glm::normalize(vector);
                    glm::vec3 endPoint = vector + glm::vec3(x,y,z);                                             // The endpoint will be (x,y,z) + f(x,y,z)
                    temp = Bresenham::drawLine(glm::vec3(x,y,z) , endPoint , lineColor);                        // we get the coordinates to represent the vector
                    arrowPixels = getArrowPixels(endPoint , vector , lineColor);                                // we get the coordinates for the arrowHead
                    temp.insert(temp.end() , arrowPixels.begin() , arrowPixels.end());
                    pixels.insert(pixels.end() , temp.begin() , temp.end());
                    temp = Bresenham::drawCircle(glm::vec3(x,y,z) + glm::vec3(0.5f)*vector , glm::length(vector)/2 , circleColor);              // we get the coordinates for the circle
                    pixels.insert(pixels.end() , temp.begin() , temp.end());
                }
            }
        break;
        case 3:                                                     // POLYLINE MODE
            float x = polyline_start.x;                             // polyline beginning X
            float y = polyline_start.y;                             // polyline beginning y
            float z = polyline_start.z;                             // polyline beginning z
            for(int i = 0 ; i < polyline_count ; i++)               // we make polyline_count number of lines.
            {
                glm::vec3 vector = getVector(x,y,z);                                                                // The value of the vector field at (x,y,z)
                if(vector != glm::vec3(0.0f) && normalize)vector = glm::normalize(vector);
                glm::vec3 endPoint = vector + glm::vec3(x,y,z);                                                     // The endpoint will be (x,y,z) + f(x,y,z)
                temp = Bresenham::drawLine(glm::vec3(x,y,z) , endPoint , lineColor);                                // we get the coordinates to represent the vector
                arrowPixels = getArrowPixels(endPoint , vector , lineColor);
                temp.insert(temp.end() , arrowPixels.begin() , arrowPixels.end());                                  // we get the coordinates for the arrowHead
                pixels.insert(pixels.end() , temp.begin() , temp.end());

                //We know change the starting coordinates to be the end coordinates.
                x = endPoint.x;
                y = endPoint.y;
                z = endPoint.z;
            }
        break;
    }

    
}

vector<float>getArrowPixels(glm::vec3 endPoint , glm::vec3 vec_dir , vector<float>color)
{
    vector<float>arrow;
    vector<float>temp;
    glm::vec3 arrowDir = glm::vec3(rotA * glm::vec4(vec_dir * -1.0f, 1.0f)) * 0.1f;
    arrow = Bresenham::drawLine(endPoint , endPoint + arrowDir ,color);
    arrowDir = glm::vec3(rotC * glm::vec4(vec_dir * -1.0f , 1.0f)) * 0.1f;
    temp = Bresenham::drawLine(endPoint , endPoint + arrowDir ,color);
    arrow.insert(arrow.end() , temp.begin() , temp.end());
    return arrow;
}

// This function returns the value of the vector field at a point (x,y,z)
glm::vec3 getVector(float x , float y , float z)
{
    return glm::vec3(-x , -y , z);
}
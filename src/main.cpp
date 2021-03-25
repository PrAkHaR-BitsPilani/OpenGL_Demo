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
#include "external/glm/gtc/type_ptr.hpp"

#include "external/imgui/imgui.h"
#include "external/imgui/imgui_impl_glfw.h"
#include "external/imgui/imgui_impl_opengl3.h"

using namespace std;

void processInput(GLFWwindow *window);
void mouseCallBack(GLFWwindow* window, double xpos, double ypos);
void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset);
void showDebugWindow();

// window settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera settings
glm::vec3 cameraPos(0.0f , 0.0f , 6.0f);
Camera camera(cameraPos);
float mouseLastX = SCR_WIDTH / 2.0;
float mouseLastY = SCR_HEIGHT / 2.0;
bool firstMouse = true;
bool locked = true;

//frame timing
float deltaTime = 0.0f;

float camSpeed = 2.5f;

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
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Prakhar6969", glfwGetPrimaryMonitor(), NULL);
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
    camera.setSpeed(camSpeed);

    vector<float>wall[2];
    //back wall
    wall[0] = {
        -0.5,   0.0,    -0.5,    0.0,    0.0,    1.0,
        0.5,    0.0,    -0.5,    0.0,    0.0,    1.0,
        0.5,    1.0,    -0.5,    0.0,    0.0,    1.0,
        0.5,    1.0,    -0.5,    0.0,    0.0,    1.0,
        -0.5,   1.0,    -0.5,    0.0,    0.0,    1.0,
        -0.5,   0.0,    -0.5,    0.0,    0.0,    1.0,
        //right wall
        0.5,    0.0,    -0.5,    -1.0,    0.0,    0.0,
        0.5,    1.0,    -0.5,    -1.0,    0.0,    0.0,
        0.5,    1.0,     0.5,    -1.0,    0.0,    0.0,
        0.5,    1.0,     0.5,    -1.0,    0.0,    0.0,
        0.5,    0.0,     0.5,    -1.0,    0.0,    0.0,
        0.5,    0.0,    -0.5,    -1.0,    0.0,    0.0,
        //left wall
        -0.5,   0.0,    -0.5,    1.0,    0.0,    0.0,
        -0.5,   1.0,    -0.5,    1.0,    0.0,    0.0,
        -0.5,   1.0,     0.5,    1.0,    0.0,    0.0,
        -0.5,   1.0,     0.5,    1.0,    0.0,    0.0,
        -0.5,   0.0,     0.5,    1.0,    0.0,    0.0,
        -0.5,   0.0,    -0.5,    1.0,    0.0,    0.0
    };
    wall[1] = {
        -0.5,   1.0,   -0.5,    0.0,    -1.0,    0.0,
         0.5,   1.0,   -0.5,    0.0,    -1.0,    0.0,
         0.5,   1.0,    0.5,    0.0,    -1.0,    0.0,
         0.5,   1.0,    0.5,    0.0,    -1.0,    0.0,
        -0.5,   1.0,    0.5,    0.0,    -1.0,    0.0,
        -0.5,   1.0,   -0.5,    0.0,    -1.0,    0.0,
        //floor
        -0.5,   0.0,   -0.5,    0.0,    1.0,    0.0,
         0.5,   0.0,   -0.5,    0.0,    1.0,    0.0,
         0.5,   0.0,    0.5,    0.0,    1.0,    0.0,
         0.5,   0.0,    0.5,    0.0,    1.0,    0.0,
        -0.5,   0.0,    0.5,    0.0,    1.0,    0.0,
        -0.5,   0.0,   -0.5,    0.0,    1.0,    0.0
    };

    vector<float> cube_positions = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    VertexArray* VAO_object = new VertexArray();
    VertexBufferLayout* layout_object = new VertexBufferLayout();
    layout_object -> push<float>(3);
    layout_object -> push<float>(3);
    VertexBuffer* VBO_object = new VertexBuffer(cube_positions.data() , cube_positions.size() * sizeof(float));

    VAO_object -> addBuffer(*VBO_object , *layout_object);

    int sourceCount = 4;
    glm::vec3 lightPos[sourceCount];

    lightPos[0] = glm::vec3(3.0f , 4.8f , 3.0f);
    lightPos[1] = glm::vec3(3.0f , 4.8f , -3.0f);
    lightPos[2] = glm::vec3(-3.0f , 4.8f , 3.0f);
    lightPos[3] = glm::vec3(-3.0f , 4.8f , -3.0f);

    glm::vec3 lightColor(1.0f , 1.0f , 1.0f);
    float constant = 1.0f;
    float linear = 0.07f;
    float quadratic = 0.017f;

    Shader* shader_object = new Shader("res/shaders/phong.shader");
    shader_object -> bind();
    for(int i = 0 ; i < 4 ; i++)
    {   
        shader_object -> setUniform3f("pointLights["+to_string(i)+"].position" , lightPos[i].x , lightPos[i].y , lightPos[i].z);
        shader_object -> setUniform3f("pointLights["+to_string(i)+"].ambient",  lightColor.r , lightColor.g , lightColor.b);
        shader_object -> setUniform3f("pointLights["+to_string(i)+"].diffuse",  lightColor.r , lightColor.g , lightColor.b);
        shader_object -> setUniform3f("pointLights["+to_string(i)+"].specular", lightColor.r , lightColor.g , lightColor.b);
    }
    
    


    Shader* shader_lightSource = new Shader("res/shaders/lightSource.shader");

    glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()) , 1920.0f/1080.0f , 0.1f , 100.0f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 lightSource[sourceCount];
    for(int i = 0 ; i < 4 ;i++)
        {
            lightSource[i] = glm::mat4(1.0f); lightSource[i] = glm::translate(lightSource[i] , lightPos[i]); lightSource[i] = glm::scale(lightSource[i] , glm::vec3(0.5f , 0.01f , 0.5f));
        }

    glm::mat4 mdl_wall = glm::mat4(1.0f); mdl_wall = glm::scale(mdl_wall , glm::vec3(7.0f , 5.0f , 7.0f));

    float tableThickness = 0.05;
    float tableWidth = 1;
    float tableLength = 2;
    float tableLegLength = 0.2;
    float restHeight = 1;
    
    glm::mat4 tableTop = glm::mat4(1.0f); tableTop = glm::translate(tableTop , glm::vec3(0.0 , tableLegLength , 0.0)); tableTop = glm::scale(tableTop , glm::vec3(tableLength , tableThickness , tableWidth));
    glm::mat4 tableLeg[4];
    tableLeg[0] = glm::mat4(1.0f); tableLeg[0] = glm::translate(tableLeg[0] , glm::vec3(tableLength/2 - tableThickness/2, tableLegLength/2 , -(tableWidth/2-tableThickness/2))); tableLeg[0] = glm::scale(tableLeg[0] , glm::vec3(tableThickness , tableLegLength , tableThickness));
    tableLeg[1] = glm::mat4(1.0f); tableLeg[1] = glm::translate(tableLeg[1] , glm::vec3(-(tableLength/2 - tableThickness/2), tableLegLength/2 , -(tableWidth/2-tableThickness/2))); tableLeg[1] = glm::scale(tableLeg[1] , glm::vec3(tableThickness, tableLegLength , tableThickness));
    tableLeg[2] = glm::mat4(1.0f); tableLeg[2] = glm::translate(tableLeg[2] , glm::vec3(-(tableLength/2 - tableThickness/2), tableLegLength/2 , tableWidth/2-tableThickness/2)); tableLeg[2] = glm::scale(tableLeg[2] , glm::vec3(tableThickness , tableLegLength , tableThickness));
    tableLeg[3] = glm::mat4(1.0f); tableLeg[3] = glm::translate(tableLeg[3] , glm::vec3(tableLength/2 - tableThickness/2, tableLegLength/2 , tableWidth/2-tableThickness/2)); tableLeg[3] = glm::scale(tableLeg[3] , glm::vec3(tableThickness , tableLegLength , tableThickness));
    glm::mat4 backRest = glm::mat4(1.0f); backRest = glm::translate(backRest , glm::vec3(tableLength/2-tableThickness/2 , tableLegLength + restHeight/2, 0));; backRest = glm::scale(backRest , glm::vec3(tableThickness , restHeight , tableWidth));



    float max = 40.0f;
    vector<float>axes_positions = {
        max  , 0.0f , 0.0f , 1.0f , 1.0f , 1.0f,
        -max , 0.0f , 0.0f , 1.0f , 1.0f , 1.0f,
        0.0f , max  , 0.0f , 1.0f , 1.0f , 1.0f,
        0.0f , -max , 0.0f , 1.0f , 1.0f , 1.0f,
        0.0f , 0.0f , max  , 1.0f , 1.0f , 1.0f,
        0.0f , 0.0f , -max , 1.0f , 1.0f , 1.0f
    };
    VertexArray* VAO_axes = new VertexArray();
    VertexBuffer* VBO_2 = new VertexBuffer(axes_positions.data() , axes_positions.size() * sizeof(float));
    VertexBufferLayout* layout_2 = new VertexBufferLayout();
    layout_2->push<float>(3);
    layout_2->push<float>(3);
    VAO_axes->addBuffer(*VBO_2 , *layout_2);
    Shader* shader_axes = new Shader("res/shaders/basic.shader");



    //Setup IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window, true);    
    ImGui_ImplOpenGL3_Init("#version 330");  


    Renderer renderer;

    // GLCall(glEnable(GL_BLEND));
    // GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_DEPTH_TEST));

    glm::mat4 rotMat(1.0f);

    GLCall(glBindVertexArray(0));

    // render loop  
    // -----------
    while (!glfwWindowShouldClose(window))
    {   
        processInput(window);

        GLCall(glClearColor(0.0f , 0.0f , 0.0f , 1.0f));
        renderer.clear();   

        proj = glm::perspective(glm::radians(camera.getFOV()) , 1920.0f/1080.0f , 0.1f , 100.0f);

        view = camera.getViewMatrix();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        showDebugWindow();
        ImGui::SliderFloat("Table Width" , &tableWidth , 0 , 5);
        ImGui::SliderFloat("Table Length" , &tableLength , 0 , 5);
        ImGui::SliderFloat("Table Thickness" , &tableThickness , 0 , 5);
        ImGui::SliderFloat("Table Leg Length" , &tableLegLength , 0 , 5);
        ImGui::SliderFloat("Backrest height" , &restHeight , 0 , 5);
        ImGui::Spacing();
        ImGui::ColorEdit3("Source Color" , glm::value_ptr(lightColor));
        ImGui::DragFloat("Constant" , &constant , 0.001 , 0.1 , 5);
        ImGui::DragFloat("Linear" , &linear , 0.001 , 0 , 1);
        ImGui::DragFloat("Quadratic" , &quadratic , 0.0001 , 0 , 1 , "%.4f");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        

        // {
        //     VAO_axes -> bind();
        //     shader_axes -> bind();
        //     shader_axes -> setUniformMat4f("u_MVP" , proj * view);
        //     GLCall(glDrawArrays(GL_LINES, 0 , 6));
        // }
        
        VBO_object -> bind();
        VBO_object -> updateData(cube_positions.data() , cube_positions.size() * sizeof(float));
        VAO_object -> addBuffer(*VBO_object , *layout_object);

        {
            shader_lightSource -> bind();
            for(int i = 0 ; i < sourceCount ; i++)
            {
                glm::mat4 MVP = proj * view * lightSource[i];
                shader_lightSource -> setUniformMat4f("u_MVP" , MVP);
                shader_lightSource -> setUniform3f("lightColor" , lightColor.r , lightColor.g , lightColor.b);
                GLCall(glDrawArrays(GL_TRIANGLES , 0 ,36));
            }
            //renderer.draw(*VAO[0] , *IBO[0] , *shader);
        }

        shader_object -> bind();
        shader_object -> setUniformMat4f("proj" , proj);
        shader_object -> setUniformMat4f("view" , view);
        shader_object -> setUniform3f("viewPos" , camera.getPosition().x , camera.getPosition().y , camera.getPosition().z);
        shader_object -> setUniform3f("material.ambient",0.0,0.1,0.06);
        shader_object -> setUniform3f("material.diffuse",0.0,0.50980392,0.50980392);
        shader_object -> setUniform3f("material.specular",0.50196078,0.50196078,0.50196078);
        shader_object -> setUniform1f("material.shininess",0.25 * 128);
        for(int i = 0 ; i < sourceCount ; i++)
        {   
            shader_object -> setUniform3f("pointLights["+to_string(i)+"].ambient",  lightColor.r , lightColor.g , lightColor.b);
            shader_object -> setUniform3f("pointLights["+to_string(i)+"].diffuse",  lightColor.r , lightColor.g , lightColor.b);
            shader_object -> setUniform3f("pointLights["+to_string(i)+"].specular", lightColor.r , lightColor.g , lightColor.b);
            shader_object -> setUniform1f("pointLights["+to_string(i)+"].a_constant" , constant);
            shader_object -> setUniform1f("pointLights["+to_string(i)+"].a_linear" , linear);
            shader_object -> setUniform1f("pointLights["+to_string(i)+"].a_quadratic" , quadratic);
        }

        shader_object -> setUniformMat4f("model" , mdl_wall);

        {
            VBO_object -> updateData(wall[0].data() , wall[0].size() * sizeof(float));
            VAO_object -> addBuffer(*VBO_object , *layout_object);
            GLCall(glDrawArrays(GL_TRIANGLES , 0 ,18));
        }
        shader_object -> setUniform3f("material.ambient",0.25,0.25,0.25);
        shader_object -> setUniform3f("material.diffuse",0.4,0.4,0.4);
        shader_object -> setUniform3f("material.specular",0.0,0.0,0.0);
        shader_object -> setUniform1f("material.shininess",0.6 * 128);
        {
            VBO_object -> updateData(wall[1].data() , wall[1].size() * sizeof(float));
            VAO_object -> addBuffer(*VBO_object , *layout_object);
            GLCall(glDrawArrays(GL_TRIANGLES , 0 ,12));
        }
        shader_object -> setUniform3f("material.ambient",0.135,0.2225,0.1575);
        shader_object -> setUniform3f("material.diffuse",0.54,0.89,0.63);
        shader_object -> setUniform3f("material.specular",0.316228,0.316228,0.316228);
        shader_object -> setUniform1f("material.shininess",0.1 * 128);

        tableTop = glm::mat4(1.0f); tableTop = glm::translate(tableTop , glm::vec3(0.0 , tableLegLength , 0.0)); tableTop = glm::scale(tableTop , glm::vec3(tableLength , tableThickness , tableWidth));
        tableLeg[0] = glm::mat4(1.0f); tableLeg[0] = glm::translate(tableLeg[0] , glm::vec3(tableLength/2 - tableThickness/2, tableLegLength/2 , -(tableWidth/2-tableThickness/2))); tableLeg[0] = glm::scale(tableLeg[0] , glm::vec3(tableThickness , tableLegLength , tableThickness));
        tableLeg[1] = glm::mat4(1.0f); tableLeg[1] = glm::translate(tableLeg[1] , glm::vec3(-(tableLength/2 - tableThickness/2), tableLegLength/2 , -(tableWidth/2-tableThickness/2))); tableLeg[1] = glm::scale(tableLeg[1] , glm::vec3(tableThickness, tableLegLength , tableThickness));
        tableLeg[2] = glm::mat4(1.0f); tableLeg[2] = glm::translate(tableLeg[2] , glm::vec3(-(tableLength/2 - tableThickness/2), tableLegLength/2 , tableWidth/2-tableThickness/2)); tableLeg[2] = glm::scale(tableLeg[2] , glm::vec3(tableThickness , tableLegLength , tableThickness));
        tableLeg[3] = glm::mat4(1.0f); tableLeg[3] = glm::translate(tableLeg[3] , glm::vec3(tableLength/2 - tableThickness/2, tableLegLength/2 , tableWidth/2-tableThickness/2)); tableLeg[3] = glm::scale(tableLeg[3] , glm::vec3(tableThickness , tableLegLength , tableThickness));
        backRest = glm::mat4(1.0f); backRest = glm::translate(backRest , glm::vec3(tableLength/2-tableThickness/2 , tableLegLength + restHeight/2, 0));; backRest = glm::scale(backRest , glm::vec3(tableThickness , restHeight , tableWidth));

        {
            VBO_object -> updateData(cube_positions.data() , cube_positions.size() * sizeof(float));
            VAO_object -> addBuffer(*VBO_object , *layout_object);
            shader_object -> setUniformMat4f("model" , tableTop);
            GLCall(glDrawArrays(GL_TRIANGLES , 0 ,36));
            shader_object -> setUniformMat4f("model" , tableLeg[0]);
            GLCall(glDrawArrays(GL_TRIANGLES , 0 ,36));
            shader_object -> setUniformMat4f("model" , tableLeg[1]);
            GLCall(glDrawArrays(GL_TRIANGLES , 0 ,36));
            shader_object -> setUniformMat4f("model" , tableLeg[2]);
            GLCall(glDrawArrays(GL_TRIANGLES , 0 ,36));
            shader_object -> setUniformMat4f("model" , tableLeg[3]);
            GLCall(glDrawArrays(GL_TRIANGLES , 0 ,36));
            shader_object -> setUniformMat4f("model" , backRest);
            GLCall(glDrawArrays(GL_TRIANGLES , 0 ,36));
        }

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
}
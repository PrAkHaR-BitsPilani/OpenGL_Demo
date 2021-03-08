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

// window settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

// camera settings
Camera camera(glm::vec3(0.0f , 0.0f , 6.0f));
float mouseLastX = SCR_WIDTH / 2.0;
float mouseLastY = SCR_HEIGHT / 2.0;
bool firstMouse = true;

//frame timing
float deltaTime = 0.0f;


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
    glfwSetInputMode(window , GLFW_CURSOR , GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window , mouseCallBack); // for mouse movements
    glfwSetScrollCallback(window , scrollCallBack); // for mouse scrolls
    // glad: load all OpenGL function pointers
    // ---------------------------------------

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    cout << glGetString(GL_VERSION) << "\n";

    glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()) , 1600.0f/900.0f , 0.1f , 100.0f);
    glm::mat4 view = camera.getViewMatrix();

    Shader* shader = new Shader("res/shaders/Basic.shader");
    VertexArray* sampleVAO = new VertexArray();
    VertexBufferLayout* sampleLayout = new VertexBufferLayout();
    sampleLayout -> push<float>(3);
    vector<float>pixels = Bresenham::drawAxis(10);
    vector<float> temp;
    for(int x = -2 ; x <= 2 ; x++)
    {
        for(int y  = -2 ; y <= 2 ;y++)
        {
            temp = Bresenham::drawLine(glm::vec3(x,y,1.0f) , glm::vec3(2*x,2*y,1.0f));
            pixels.insert(pixels.end() , temp.begin() , temp.end());
        }
    }
    VertexBuffer* sampleVBO = new VertexBuffer(pixels.data() , pixels.size() * sizeof(float));
    sampleVAO -> addBuffer(*sampleVBO , *sampleLayout);
    GLCall(glBindVertexArray(0));

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

    // render loop  
    // -----------
    while (!glfwWindowShouldClose(window))
    {   
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f,1.0f);
        renderer.clear();   

        proj = glm::perspective(glm::radians(camera.getFOV()) , 1600.0f/900.0f , 0.1f , 100.0f);
        view = camera.getViewMatrix();

        // IMGUI NEW FRAME
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        shader -> bind();
        shader -> setUniformMat4f("u_MVP" , proj * view);
        renderer.drawPoints(*sampleVAO , *shader , pixels.size());

        {
            //ImGui::ShowDemoWindow();
            ImGui::Begin("debug");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

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
}

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

void scrollCallBack(GLFWwindow* window, double x_offset, double y_offset)
{
    camera.processMouseScroll(y_offset);
}
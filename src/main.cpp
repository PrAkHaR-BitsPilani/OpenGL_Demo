#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "external/glm/glm.hpp"
#include "external/glm/gtc/matrix_transform.hpp"

#include "external/imgui/imgui.h"
#include "external/imgui/imgui_impl_glfw.h"
#include "external/imgui/imgui_impl_opengl3.h"

using namespace std;

void processInput(GLFWwindow *window);
void mouseCallBack(GLFWwindow* window, double xpos, double ypos);
void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

// camera settings
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);  
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
const float cameraSpeed = 2.5f;

// mouse settings
float mouseLastX = SCR_WIDTH / 2.0;
float mouseLastY = SCR_HEIGHT / 2.0;
float yaw = -90.0f;
float pitch = 0.0;
float fov = 45.0;
const float sensitivity = 0.1f;
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
    glfwSetCursorPosCallback(window , mouseCallBack);
    glfwSetScrollCallback(window , scrollCallBack);
    // glad: load all OpenGL function pointers
    // ---------------------------------------

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    cout << glGetString(GL_VERSION) << "\n";

    vector<float> positions = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    vector<unsigned int> indices= {
        0,1,2,
        2,3,0
    };

    VertexArray* VAO[1];
    VertexBuffer* VBO[1];
    IndexBuffer* IBO[1];

    VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(2);

    VAO[0] = new VertexArray();
    VBO[0] = new VertexBuffer(positions.data()  , positions.size() * sizeof(float));
    VAO[0] -> addBuffer(*VBO[0] , layout);
    IBO[0] = new IndexBuffer(indices.data() , indices.size());

    glBindVertexArray(0);

    glm::mat4 proj = glm::perspective(glm::radians(fov) , 1600.0f/900.0f , 0.1f , 100.0f);
    glm::mat4 view = glm::lookAt(cameraPos , cameraPos + cameraFront , cameraUp);

    Shader* shader = new Shader("res/shaders/Basic.shader");
    shader -> bind();
    shader -> setUniform1i("u_Texture" , 0);

    Texture* black = new Texture("res/textures/images/black.jpg");
    Texture* red = new Texture("res/textures/images/red.jpg");
    
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

        glClearColor(0.4f, 0.4f, 0.4f,1.0f);
        renderer.clear();

        VAO[0] -> bind();   

        proj = glm::perspective(glm::radians(fov) , 1600.0f/900.0f , 0.1f , 100.0f);
        view = glm::lookAt(cameraPos , cameraPos + cameraFront , cameraUp);

        // IMGUI NEW FRAME
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            glm::mat4 MVP = proj * view * model;
            shader -> setUniformMat4f("u_MVP" , MVP);
            black -> bind();
            GLCall(glDrawArrays(GL_TRIANGLES , 0 ,36));
            //renderer.draw(*VAO[0] , *IBO[0] , *shader);
        }

        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model , glm::vec3(-2.5f , 0 , -2.0f));
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            glm::mat4 MVP = proj * view * model;
            shader -> setUniformMat4f("u_MVP" , MVP);
            red -> bind();
            GLCall(glDrawArrays(GL_TRIANGLES , 0 ,36));
            //renderer.draw(*VAO[0] , *IBO[0] , *shader);
        }

        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model , glm::vec3(1.5f , 0 , 0));
            model = glm::scale(model , glm::vec3(0.5f));
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            glm::mat4 MVP = proj * view * model;
            shader -> setUniformMat4f("u_MVP" , MVP);
            GLCall(glDrawArrays(GL_TRIANGLES , 0 ,36));
            //renderer.draw(*VAO[1] , *IBO[0] , *shader);
        }

        {
            ImGui::ShowDemoWindow();
            //ImGui::SliderFloat2("Camera" , &camera_translate[0] , -2.0f , 2.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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
    float speed = cameraSpeed * deltaTime;
    deltaTime = glfwGetTime(); 

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += speed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= speed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
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
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    yaw += x_offset;
    pitch += y_offset;

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront.y = sin(glm::radians(pitch));
    cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
}

void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f; 
}
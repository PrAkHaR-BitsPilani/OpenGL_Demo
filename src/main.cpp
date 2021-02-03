#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

using namespace std;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const float OFFSET = 0.5f;

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
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", NULL, NULL);
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

    vector<float> positions_back = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f , 0.5f
    };
    vector<float>position_front;
    for(int i = 0 ; i < 8 ; i++)position_front.push_back(positions_back[i] - 0.5f);
    vector<float>position_left = {
        0.0f, 0.0f,
        0.5f, 0.5f,
        0.0f, -1.0f,
        0.5f, -0.5f
    };
    vector<float>position_right = {
        -1.0f, 0.0f,
        -0.5f, 0.5f,
        -0.5f, 0.0f
    };

    vector<unsigned int> indices_front_back= {
        0,1,2,
        2,3,0
    };
    vector<unsigned int>indices_left{
        0,1,2,
        1,2,3
    };
    vector<unsigned int>indices_right{
        0,1,2
    };

    for(int i = 0 ; i < 8 ; i++){
        position_front[i] += OFFSET;
        positions_back[i] += OFFSET;
        position_left[i] += OFFSET;
    }
    for(int i = 0 ; i < 6; i++){
        position_right[i] += OFFSET;
    }

    VertexArray* VAO[4];
    VertexBuffer* VBO[4];
    IndexBuffer* IBO[3];

    VertexBufferLayout layout;
    layout.push<float>(2);

    VAO[0] = new VertexArray();
    VBO[0] = new VertexBuffer(positions_back.data()  , positions_back.size() * sizeof(float));
    VAO[0] -> addBuffer(*VBO[0] , layout);
    IBO[0] = new IndexBuffer(indices_front_back.data() , indices_front_back.size());

    glBindVertexArray(0);

    VAO[1] = new VertexArray();
    VBO[1] = new VertexBuffer(position_front.data() , position_front.size() * sizeof(float));
    VAO[1] -> addBuffer(*VBO[1] , layout);
    IBO[0] -> bind();

    glBindVertexArray(0);

    VAO[2] = new VertexArray();
    VBO[2] = new VertexBuffer(position_left.data() , position_left.size() * sizeof(float));
    VAO[2] -> addBuffer(*VBO[2] , layout);
    IBO[1] = new IndexBuffer(indices_left.data() , indices_left.size());

    glBindVertexArray(0);

    VAO[3] = new VertexArray();
    VBO[3] = new VertexBuffer(position_right.data() , position_right.size() * sizeof(float));
    VAO[3] -> addBuffer(*VBO[3] , layout);
    IBO[2] = new IndexBuffer(indices_right.data() , indices_right.size());

    Shader* shader = new Shader("res/shaders/Basic.shader");
    shader -> bind();
    shader -> setUniform4f("u_Color" ,  0.27f , 0.27f , 0.27f , 1.0f);

    glBindVertexArray(0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {   
        glClearColor(0.4f, 0.4f, 0.4f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES , 0 , 3);
        VAO[0] -> bind();   // back side
        shader -> setUniform4f("u_Color" , 0.1f , 0.1f , 0.1f , 1.0f);
        glDrawElements(GL_TRIANGLES, 6 , GL_UNSIGNED_INT , nullptr);
        VAO[1] -> bind();   // front side
        shader -> setUniform4f("u_Color" , 0.27f , 0.27f , 0.27f , 1.0f);
        glDrawElements(GL_TRIANGLES, 6 , GL_UNSIGNED_INT , nullptr);
        VAO[2] -> bind();   // left side
        shader -> setUniform4f("u_Color" , 0.37f , 0.37f , 0.37f , 1.0f);
        glDrawElements(GL_TRIANGLES, 6 , GL_UNSIGNED_INT , nullptr);
        VAO[3] -> bind();   // right side
        shader -> setUniform4f("u_Color" , 0.15f , 0.15f , 0.15f , 1.0f);
        GLCall(glDrawElements(GL_TRIANGLES, 3 , GL_UNSIGNED_INT , nullptr));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
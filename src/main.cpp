#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>

using namespace std;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const float OFFSET = 0.5f;

static void GLClearError(){
    while(glGetError() != GL_NO_ERROR);
}

static void GLCheckError(){
    while(GLenum error = glGetError()){
        cout << "[OpenGL ERROR]" << error << "\n"; 
    }
}

struct shaderSource{
    string VertexSource;
    string FragementSource;
};

static shaderSource parseShader(const string& filePath){

    enum class ShaderType{
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };
    
    ShaderType type = ShaderType::NONE;
    ifstream stream(filePath);
    string line;
    stringstream ss[2];
    while(getline(stream , line)){
        if(line.find("#shader") != string::npos){
            if(line.find("vertex") != string::npos)
                type = ShaderType::VERTEX;
            else if(line.find("fragment") != string::npos)
                type = ShaderType::FRAGMENT;
        }
        else{
            ss[(int)type] << line << "\n";
        }
    }
    return {ss[0].str() , ss[1].str()}; 
}

static unsigned int compileShader(unsigned int type, const string &source)
{
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    
    if (result == GL_FALSE)
    {

        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)malloc(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        cout << "Error in " << ((type == GL_VERTEX_SHADER) ? "vertex" : "fragment") << " shade" << endl;
        cout << message << endl;
        glDeleteShader(id);
        return -1;
    }

    return id;
}

static unsigned int createShader(const string &vertexShader, const string &fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    assert(vs != -1);
    assert(fs != -1);

    glAttachShader(program, fs);
    glAttachShader(program, vs);
    glLinkProgram(program);
    glValidateProgram(program);

    return program;
}

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

    unsigned int VAO[4] , VBO[4] , IBO[3];

    glGenVertexArrays(4, VAO);
    glGenBuffers(4, VBO);
    glGenBuffers(3, IBO);

    glBindVertexArray(VAO[0]);     //  Binding Vertex Array Object. Since this is the first time it will have the default VAO state.
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);     //  Binding a Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, positions_back.size() * sizeof(float) , positions_back.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);       //  Specify the layout of the vertex data 
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float) , 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[0]);     //  Binding an Index Buffer Object
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_front_back.size() * sizeof(unsigned int) , indices_front_back.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, position_front.size() * sizeof(float) , position_front.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float) , 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[0]);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_front_back.size() * sizeof(unsigned int) , indices_front_back.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER , VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, position_left.size() * sizeof(float) , position_left.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float) , 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , IBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_left.size() * sizeof(unsigned int) , indices_left.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER , VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, position_right.size() * sizeof(float) , position_right.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float) , 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , IBO[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_right.size() * sizeof(unsigned int) , indices_right.data(), GL_STATIC_DRAW);


    shaderSource source = parseShader("res/shaders/Basic.shader");
    unsigned int shader = createShader(source.VertexSource, source.FragementSource);
    glUseProgram(shader);

    int location = glGetUniformLocation(shader , "u_Color");
    glUniform4f(location , 0.27f , 0.27f , 0.27f , 1.0f);

    glBindVertexArray(0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {   
        glClearColor(0.4f, 0.4f, 0.4f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES , 0 , 3);
        glBindVertexArray(VAO[0]); // back side
        glUniform4f(location , 0.1f , 0.1f , 0.1f , 1.0f);
        glDrawElements(GL_TRIANGLES, 6 , GL_UNSIGNED_INT , nullptr);
        glBindVertexArray(VAO[1]); // front side
        glUniform4f(location , 0.27f , 0.27f , 0.27f , 1.0f);
        glDrawElements(GL_TRIANGLES, 6 , GL_UNSIGNED_INT , nullptr);
        glBindVertexArray(VAO[2]); // left side
        glUniform4f(location , 0.37f , 0.37f , 0.37f , 1.0f);
        glDrawElements(GL_TRIANGLES, 6 , GL_UNSIGNED_INT , nullptr);
        glBindVertexArray(VAO[3]);
        glUniform4f(location , 0.15f , 0.15f , 0.15f , 1.0f);
        glDrawElements(GL_TRIANGLES, 3 , GL_UNSIGNED_INT , nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFWresources.
    //---------------------------------------------------------------
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
#shader vertex
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 iColor;

out vec3 oColor;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(position,1.0);
    oColor = iColor;
};

#shader fragment
#version 330 core
layout(location = 0)out vec4 color;

in vec3 oColor;

void main()
{
    color = vec4(oColor , 1.0f);
};
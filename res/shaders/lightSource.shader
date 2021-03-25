#shader vertex
#version 330 core
layout(location = 0) in vec3 position;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(position,1.0);
};

#shader fragment
#version 330 core
layout(location = 0)out vec4 Fragcolor;

uniform vec3 lightColor;

void main()
{
    Fragcolor = vec4(lightColor, 1.0f);
};
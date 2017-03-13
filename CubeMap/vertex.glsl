#version 330 core

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
layout(location = 0) in vec3 position;
uniform mat4 project;
uniform mat4 view;
uniform mat4 model;
out vec3 TexCoords;
void main()
{
    gl_Position = project * view * model * vec4(position, 1.0);
    TexCoords = position;
}

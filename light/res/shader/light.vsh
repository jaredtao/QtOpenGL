#version 330 core

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectMat;

layout (location = 0) in vec4 a_position;
layout (location = 1) in vec2 a_texcoord;
layout (location = 2) in vec2 a_normal;

out vec2 v_texcoord;

void main(void)
{
    v_texcoord = a_texcoord;
    gl_Position = projectMat * viewMat * modelMat * a_position;
}

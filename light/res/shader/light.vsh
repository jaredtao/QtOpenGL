#version 330 core

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectMat;

in vec4 a_position;
in vec2 a_texcoord;

out vec2 v_texcoord;
void main(void)
{
    v_texcoord = a_texcoord;
    gl_Position = projectMat * viewMat * modelMat * a_position;
}

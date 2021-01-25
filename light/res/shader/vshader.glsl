#version 330 core

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectMat;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texcoord;
layout (location = 2) in vec3 a_normal;

out vec2 v_texcoord;
out vec3 v_normal;
out vec3 v_fragPos;

void main()
{
    // Calculate vertex position in screen space
    v_texcoord  = a_texcoord;
//    v_normal    = mat3(transpose(inverse(modelMat))) *a_normal;
    v_normal = a_normal;
    gl_Position = projectMat * viewMat * modelMat * vec4(a_position, 1.0f);
    v_fragPos   = vec3(modelMat * vec4(a_position, 1.0f));
}


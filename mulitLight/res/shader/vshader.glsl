//#version 430 core
#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectMat;

attribute vec3 a_position;
attribute vec2 a_texcoord;
attribute vec3 a_normal;

varying vec2 v_texcoord;
varying vec3 v_normal;
varying vec3 v_fragPos;
void main()
{
    // Calculate vertex position in screen space
    v_texcoord  = a_texcoord;
    v_normal    = mat3(transpose(inverse(modelMat))) *a_normal;
//    v_normal = a_normal;
    v_fragPos   = vec3(modelMat * vec4(a_position, 1.0f));
    gl_Position = projectMat * viewMat * vec4(v_fragPos, 1.0f);
}


#version 330 core

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

layout (location = 0) in vec4 qt_Vertex;
layout (location = 1) in vec4 qt_MultiTexCoord0;
layout (location = 2) in vec4 a_Color;
uniform mat4 qt_ModelViewProjectionMatrix;
out vec4 qt_TexCoord0;
out vec4 v_Color;
void main(void)
{
    v_Color = a_Color;
    gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;
    qt_TexCoord0 = qt_MultiTexCoord0;
}

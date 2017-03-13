#version 330 core

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D qt_Texture0;
in highp vec4 qt_TexCoord0;
in vec4 v_Color;
out vec4 color;
void main(void)
{
    color = texture2D(qt_Texture0, qt_TexCoord0.st);
}

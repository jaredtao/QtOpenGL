#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif
attribute highp vec4 qt_Vertex;
attribute highp vec4 qt_MultiTexCoord0;
attribute vec4 a_Color;
uniform highp mat4 qt_ModelViewProjectionMatrix;
varying highp vec4 qt_TexCoord0;
varying vec4 v_Color;
void main(void)
{
    v_Color = a_Color;
    gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;
    qt_TexCoord0 = qt_MultiTexCoord0;
}

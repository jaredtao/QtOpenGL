#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif
uniform sampler2D qt_Texture0;
varying highp vec4 qt_TexCoord0;
varying vec4 v_Color;
void main(void)
{
//    gl_FragColor = v_Color;
    gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0.st);
}

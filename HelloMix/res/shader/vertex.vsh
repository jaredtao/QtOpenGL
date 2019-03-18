#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif
attribute highp vec3 qt_Vertex;
attribute highp vec2 texCoord;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectMatrix;

varying vec2 v_texCoord;
void main(void)
{
    gl_Position = u_projectMatrix * u_viewMatrix * u_modelMatrix * vec4(qt_Vertex, 1.0f);
    v_texCoord = texCoord;
}


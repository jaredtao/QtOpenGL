#version 330 core

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
uniform samplerCube skyBox;

in vec3 TexCoords;
out vec4 color;
void main(void)
{
    color = texture(skyBox, TexCoords);
}

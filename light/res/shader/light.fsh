#version 330 core

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
uniform vec3 lightColor;

in vec2 v_texcoord;
out vec4 fragColor;
void main(void)
{
    fragColor = vec4(lightColor, 1.0);
}

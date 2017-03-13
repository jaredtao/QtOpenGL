#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
uniform vec3 lightColor;

varying vec2 v_texcoord;

void main(void)
{
    gl_FragColor = vec4(lightColor, 1.0);
}

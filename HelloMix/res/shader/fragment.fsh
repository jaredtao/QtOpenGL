#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif
varying vec2 v_texCoord;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform float mixPara;
void main(void)
{
    gl_FragColor = mix(texture2D(u_texture1, v_texCoord), texture2D(u_texture2, v_texCoord), mixPara);

}

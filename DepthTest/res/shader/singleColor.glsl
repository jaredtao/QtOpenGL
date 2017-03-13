#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;

varying vec2 v_texcoord;

//! [0]
void main()
{
    // Set fragment color from texture
    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
//! [0]



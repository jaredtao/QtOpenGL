#version 330 core

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D texture;
uniform vec3 viewPos;


in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_fragPos;

out vec4 fragColor;
//! [0]
void main()
{


    //ambient 环境光
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    //diffuse 漫反射光
    vec3 norm     = normalize(v_normal);
    vec3 lightDir = normalize(lightPos - v_fragPos);
    float diff    = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = diff * lightColor;

    //纹理颜色
    vec4 rgb = texture(texture, v_texcoord);

    //镜面高光
    float specularStrength = 0.9f;
    vec3 viewDir = normalize(viewPos - v_fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    fragColor = vec4(ambient + diffuse + specular, 1.0) * rgb;
    fragColor = vec4(0.2, 0.3, 0.4, 1.0);


}
//! [0]


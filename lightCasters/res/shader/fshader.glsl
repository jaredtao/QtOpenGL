#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    float constant;
    float linear;
    float quadratic;

    float cutoff;
    float outerCutOff;
};
uniform Light light;
uniform Material material;

uniform vec3 viewPos;

varying vec2 v_texcoord;
varying vec3 v_normal;
varying vec3 v_fragPos;
//! [0]
void main()
{
    vec3 texDiffuse = vec3(texture2D(material.diffuse, v_texcoord));


    //ambient 环境光
    vec3 ambient =  light.ambient * texDiffuse;

    //diffuse 漫反射光
    vec3 norm     = normalize(v_normal);
    //平行光，直接用方向向量
    //    vec3 lightDir = normalize(-light.direction);
    //点光, 可以增加衰减
    vec3 lightDir = normalize(light.position - v_fragPos);

    //手电筒区域计算
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    //计算距离和衰减系数
    float distance = length(light.position - v_fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));


    float diff    = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse * diff * texDiffuse;

    //镜面高光
    float specularStrength = 0.9f;
    vec3 viewDir = normalize(viewPos - v_fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture2D(material.specular, v_texcoord));

    gl_FragColor = vec4(attenuation * (ambient + (intensity * diffuse) + (intensity * specular)), 1.0) ;

}
//! [0]


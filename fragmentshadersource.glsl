#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D normalMap;
uniform bool useTexture;          // 是否使用贴图
uniform vec3 modelColor;          // 无纹理时使用的颜色

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main()
{
    vec3 finalNormal;
    vec3 diffuseColor;

    if (useTexture) {
        // 有纹理的物体：从法线贴图采样，从漫反射贴图采样颜色
        vec3 normalFromMap = texture(normalMap, TexCoord).rgb;
        finalNormal = normalize(normalFromMap * 2.0 - 1.0);
        diffuseColor = texture(texture1, TexCoord).rgb;
    } else {
        // 无纹理的物体：使用顶点法线和模型颜色
        finalNormal = normalize(Normal);
        diffuseColor = modelColor;
    }

    // 环境光
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    // 漫反射
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(finalNormal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // 高光
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(finalNormal, halfwayDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * diffuseColor;
    FragColor = vec4(result, 1.0);
}
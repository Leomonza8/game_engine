#version 330 core

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D tex;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform bool hasTexture;

void main() {
    vec3 baseColor = hasTexture ? texture(tex, TexCoord).rgb : vec3(0.8);

    // ambient
    vec3 ambient = 0.25 * lightColor * baseColor;

    // diffuse
    vec3 norm     = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff    = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = diff * lightColor * baseColor;

    // specular
    vec3 viewDir  = normalize(viewPos - FragPos);
    vec3 reflect  = reflect(-lightDir, norm);
    float spec    = pow(max(dot(viewDir, reflect), 0.0), 32.0);
    vec3 specular = 0.3 * spec * lightColor;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}

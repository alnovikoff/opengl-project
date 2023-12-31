#version 330 core
out vec4 FragColor;

in vec3 WorldPos;
in vec3 Normal;

uniform vec3 cameraPos;
uniform samplerCube cubemap;

void main()
{
    vec3 I = normalize(WorldPos - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(cubemap, R).rgb, 1.0);
}

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    /* calculating position of the fragment
    by multiplying the model matrix with the position of the vertex 
    in 3D space */
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    /*
    calculating the final position of the vertex in clip space by 
    multiplying the projection matrix, view matrix, and the position of 
    the fragment
    */
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

#version 330 core

struct Material
{
    sampler2D diffuse;
    float shininess;
};

struct Light 
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Spotlight
{
    vec3 position;
    vec3 direction;
    float cutoff;
    float outerCutoff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Light light;
uniform Spotlight spotlight[3];
uniform Material material;
uniform float alpha; // Add this line

void main()
{
    // Apply alpha blending
    vec4 texColor = texture(material.diffuse, TexCoords);
    vec4 blendedColor = vec4(texColor.rgb, texColor.a * alpha); // Modify this line

    // Ambient
    vec3 ambient = light.ambient * blendedColor.rgb; // Modify this line

    // Diffuse from directional light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diffDirectional = max(dot(norm, lightDir), 0.0);
    vec3 diffuseDirectional = light.diffuse * diffDirectional * blendedColor.rgb; // Modify this line

    // Initialize the final color outside the loop
    vec3 result = ambient + diffuseDirectional;

    for(int i = 0; i < 3; i++)
    {
        // Diffuse from spotlight
        vec3 spotLightDir = normalize(spotlight[i].position - FragPos);
        float cosTheta = dot(spotLightDir, normalize(spotlight[i].direction));
        float spotlightEffect = smoothstep(spotlight[i].outerCutoff, spotlight[i].cutoff, cosTheta);
        float diffSpotlight = max(dot(norm, spotLightDir), 0.0);
        vec3 diffuseSpotlight = spotlight[i].diffuse * diffSpotlight * spotlightEffect * blendedColor.rgb; // Modify this line

        // Specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-spotLightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = spotlight[i].specular * (spec * vec3(1.0));

        // Accumulate the contributions from spotlight in the result
        result += diffuseSpotlight + specular;
    }

    // Assign the final color to gl_FragColor
    gl_FragColor = vec4(result, blendedColor.a); // Modify this line
}

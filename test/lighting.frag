#version 330

in vec3 normal;
in vec3 frag_pos;

out vec4 frag_color;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec4 object_color;
uniform vec3 view_pos;
// uniform vec3 light_pos;
// uniform vec3 light_color;

uniform Material material;
uniform Light light;

void main()
{
    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light.position - frag_pos);
    float diffuse_comp = max(dot(light_dir, normal), 0.0);
    vec3 diffuse = light.diffuse * (diffuse_comp * material.diffuse);

    // specular
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float specular_comp = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = light.specular * (specular_comp * material.specular);

    vec3 result = (diffuse + ambient + specular) * object_color.xyz;
    frag_color = vec4(result, 1.0);
}
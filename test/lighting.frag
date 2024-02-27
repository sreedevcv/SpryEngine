#version 330

in vec3 normal;
in vec3 frag_pos;

out vec4 frag_color;

uniform vec4 object_color;
uniform vec3 light_pos;
uniform vec3 light_color;

void main()
{
    // ambient
    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * light_color;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light_pos - frag_pos);
    float diffuse_comp = max(dot(light_dir, normal), 0.0);
    vec3 diffuse = light_color * diffuse_comp;

    vec3 result = (diffuse + ambient) * object_color.xyz;
    frag_color = vec4(result, 1.0);

    // frag_color = object_color;
}
#version 330 core

out vec4 FragColor;

in vec3 ModelLocation;

void main()
{
    vec3 Dir = normalize(ModelLocation);

    FragColor = vec4(smoothstep(1.0,1.0,Dir.x), smoothstep(-1.0,1.0,Dir.y), smoothstep(-1.0,1.0,Dir.z), 1.0);
}
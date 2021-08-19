#version 330 core

out vec4 FragColor;

void main()
{
    vec2 fromCenter = gl_PointCoord - vec2(0.5,0.5);
    float fromCenterDistance = dot(fromCenter, fromCenter);

    if(fromCenterDistance > 0.25)
    {
        discard;
    }

    FragColor = vec4(1.0, 1.0, 0.1, 0.6);
}
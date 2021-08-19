#version 330 core

layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position.x = mix(-1.0, 1.0, smoothstep(-2000.0, 2000.0, aPos.x));
    gl_Position.y = mix(-1.0, 1.0, smoothstep(-2000.0, 2000.0, aPos.y));
    gl_Position.z = mix(-1.0, 0.0, smoothstep(-2000.0, 2000.0, aPos.z));

    gl_Position.w = 1.0;

    gl_PointSize = 5;
}
#version 330 core

layout (location = 0) in vec3 aPos;

uniform vec3 cameraPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * vec4(aPos,1.0);

    vec3 fromCamera = cameraPos - aPos;
    float fromCameraDistance = sqrt(dot(fromCamera, fromCamera));

    float SphereSizeWithCameraDistanceOne = 160;

    gl_PointSize = SphereSizeWithCameraDistanceOne / fromCameraDistance;
}
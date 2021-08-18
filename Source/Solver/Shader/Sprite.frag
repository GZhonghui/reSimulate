#version 330 core

uniform sampler2D spriteTexture;

out vec4 FragColor;

void main()
{
    vec2 fromCenter = gl_PointCoord - vec2(0.5,0.5);
    float fromCenterDistance = dot(fromCenter, fromCenter);

    if(fromCenterDistance > 0.25)
    {
        discard;
    }

    FragColor = texture(spriteTexture, gl_PointCoord);
    
    float smoothLimit = 0.21;

    if(fromCenterDistance > smoothLimit)
    {
        vec4 outColor =  vec4(0.0);
        FragColor = mix(FragColor, outColor, smoothstep(smoothLimit, 0.25, fromCenterDistance));
    }
}
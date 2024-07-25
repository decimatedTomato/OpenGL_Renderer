#version 330 core
precision highp float;

in vec4 vVertexColor;
in vec2 vUV;

#define POINT_COUNT 32
#define MARKER_RADIUS 0.01

uniform float uTime;
uniform ivec2 uResolution;

uniform int uPointCount;
uniform vec2[POINT_COUNT] uPointPositions;
uniform vec3[POINT_COUNT] uPointColors;
uniform vec2[POINT_COUNT] uPointVelocities;

vec2 getPosition(int n){
    vec2 pos = mod(uPointPositions[n] + uTime * uPointVelocities[n], vec2(1));
    if (int(uPointPositions[n].x + uTime * uPointVelocities[n].x) % 2 == 0) pos.x = 1 - pos.x;
    if (int(uPointPositions[n].y + uTime * uPointVelocities[n].y) % 2 == 0) pos.y = 1 - pos.y;
    return pos;
}
void main()
{
    vec2 adjustedUV = vec2(vUV.x * uResolution.x / uResolution.y, vUV.y);

    int closestPosition = 0;
    float minDistance = length(uResolution);
    for (int i = 0; i < uPointCount; i++)
    {
        float d = distance(adjustedUV, getPosition(i));
        if (d < minDistance)
        {
            closestPosition = i;
            minDistance = d;
        }
    }
    vec3 color = uPointColors[closestPosition];
    if (minDistance < MARKER_RADIUS) color = 1 - color;
    gl_FragColor = vec4(color, 1.0);
}
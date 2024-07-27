#version 330 core
precision highp float;

in vec4 v_col;
in vec2 v_uv;

uniform float u_time;
uniform ivec2 u_resolution;

#define POINT_COUNT 32
#define MARKER_RADIUS 0.01

uniform int u_p_count;
uniform vec2[POINT_COUNT] u_p_pos;
uniform vec3[POINT_COUNT] u_p_col;
uniform vec2[POINT_COUNT] u_p_vel;

vec2 getPosition(int n){
    vec2 pos = mod(u_p_pos[n] + u_time * u_p_vel[n], vec2(1));
    if (int(u_p_pos[n].x + u_time * u_p_vel[n].x) % 2 == 0) pos.x = 1 - pos.x;
    if (int(u_p_pos[n].y + u_time * u_p_vel[n].y) % 2 == 0) pos.y = 1 - pos.y;
    return pos;
}
void main()
{
    vec2 adjustedUV = vec2(v_uv.x * u_resolution.x / u_resolution.y, v_uv.y);

    int closestPosition = 0;
    float minDistance = length(u_resolution);
    for (int i = 0; i < u_p_count; i++)
    {
        float d = distance(adjustedUV, getPosition(i));
        if (d < minDistance)
        {
            closestPosition = i;
            minDistance = d;
        }
    }
    vec3 col = u_p_col[closestPosition];
    if (minDistance < MARKER_RADIUS) col = 1 - col;
    gl_FragColor = vec4(col, 1.0);
}
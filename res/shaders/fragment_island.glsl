#version 330 core
precision highp float;

in vec4 v_col;
in vec2 v_uv;

#define COLOR_COUNT_MAX 10
#define TIME_VARYING 0

uniform float u_time;
uniform ivec2 u_resolution;
uniform float u_zoom_out;
uniform vec3[COLOR_COUNT_MAX] u_terrain_colors;
uniform int u_terrain_color_count;

/* Expects height in range 0..1 */
vec3 terrainify(float height) {
    return u_terrain_colors[int(height * u_terrain_color_count)];
}

float smootherstep(float x) {
//    if (x < 0.) return 0.;
//    if (x > 1.) return 1.;
    return 6.*x*x*x*x*x - 15.*x*x*x*x + 10.*x*x*x;
}


float random(vec2 st) {
    return fract(sin(dot(st.xy,vec2(12.9898,78.233)))*43758.5453123);
}

/* From https://www.shadertoy.com/user/CuleGamer */
vec2 random_vector(vec2 pos) {
    float theta = 3.14159 * random(pos);
#if TIME_VARYING == 1
    theta += u_time;
#endif
    return vec2(cos(theta), sin(theta));
}

void main() {
    vec2 uv = v_uv * u_resolution / u_resolution.y;

    // Repeating UV in a grid and getting cellID
    uv *= u_zoom_out;
    vec2 gridID = floor(uv);
    vec2 gridUV = fract(uv);
    
    // Finding corner coordinates of each grid cell
    vec2 bl = gridID + vec2(0, 0);
    vec2 br = gridID + vec2(1, 0);
    vec2 tl = gridID + vec2(0, 1);
    vec2 tr = gridID + vec2(1, 1);
    
    // Random gradients in 4 corners of each grid
    vec2 dbl = random_vector(bl);
    vec2 dbr = random_vector(br);
    vec2 dtl = random_vector(tl);
    vec2 dtr = random_vector(tr);
    
    // Get vector from each gradient to center of grid cell
    vec2 c2bl = gridUV - vec2(0, 0);
    vec2 c2br = gridUV - vec2(1, 0);
    vec2 c2tl = gridUV - vec2(0, 1);
    vec2 c2tr = gridUV - vec2(1, 1);
    
    // Take dot products
    float dotbl = dot(dbl, c2bl);
    float dotbr = dot(dbr, c2br);
    float dottl = dot(dtl, c2tl);
    float dottr = dot(dtr, c2tr);
    
    if ((int(u_time) / 5) % 2 == 0) {
        gridUV = vec2(smoothstep(vec2(0), vec2(1), gridUV));
    } else {
        gridUV = vec2(smootherstep(gridUV.x), smootherstep(gridUV.y));
    }
    
    // Perform linear interpolation between dot products
    float b = mix(dotbl, dotbr, gridUV.x);
    float t = mix(dottl, dottr, gridUV.x);
    float perlin = mix(b, t, gridUV.y);

    perlin = perlin * 0.875 + 0.5;
	gl_FragColor = vec4(terrainify(perlin),1.0);
//    if (perlin > 1.0) gl_FragColor = vec4(1, 0, 0, 0);
//    if (perlin < 0.0) gl_FragColor = vec4(0, 0, 1, 0);
}
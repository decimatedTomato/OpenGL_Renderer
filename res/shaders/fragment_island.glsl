#version 330 core
precision mediump float;

in vec4 v_col;
in vec2 v_uv;

#define COLOR_COUNT_MAX 10
#define TIME_VARYING 1

uniform float u_time;
uniform ivec2 u_resolution;
uniform float u_zoom_out;
uniform vec3[COLOR_COUNT_MAX] u_terrain_colors;
uniform int u_terrain_color_count;

/* Expects height in range 0..1 */
vec3 terrainify(float height) {
    height = clamp(height * 0.875 + 0.5, 0.0, 1.0);
    return u_terrain_colors[int(height * u_terrain_color_count - 0.01)];
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

float perlin_noise(vec2 uv) {
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
    
//    if ((int(u_time) / 5) % 2 == 0) {
        gridUV = vec2(smoothstep(vec2(0), vec2(1), gridUV));
//    } else {
//        gridUV = vec2(smootherstep(gridUV.x), smootherstep(gridUV.y));
//    }
    
    // Perform linear interpolation between dot products
    float b = mix(dotbl, dotbr, gridUV.x);
    float t = mix(dottl, dottr, gridUV.x);
    return mix(b, t, gridUV.y);
}

float fbm_perlin(int octaves, float falloff, vec2 uv) {
    float noise = 0.0;
    float amplitude = 1.0;
    for (int i = 0; i < octaves; i++) {
        noise += perlin_noise(uv) * amplitude;
        amplitude *= falloff;
        uv *= 2.0;
    }
    return noise;
}

void main() {
    vec2 uv = v_uv * u_resolution / u_resolution.y;
    int octaves = int(u_time) / 5 + 1;
	gl_FragColor = vec4(terrainify(fbm_perlin(octaves, 0.5, uv)), 1.0);
}
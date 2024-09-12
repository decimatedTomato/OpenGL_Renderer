#version 330 core
precision mediump float;

in vec4 v_col;
in vec2 v_uv;

uniform float u_time;
uniform ivec2 u_resolution;
uniform sampler2D u_texture;

uniform vec2 u_center;

#define SPEED 0.5
#define CENTER vec2(0.4)
#define MAX_RADIUS .8
#define DISTORTION_RADIUS 0.15
#define INTRO_END 0.1
#define OUTRO_START 0.5
#define EASE 1.5

vec2 get_offset(float t, vec2 pos, vec2 uv) {
    float r = MAX_RADIUS * t;
    float d = length(pos-uv)-r; // d is the magnitude of distortion
    vec2 dir = normalize(pos-uv);
    float distortion_mask = 1.-smoothstep(0., DISTORTION_RADIUS, abs(d));
    float smooth_intro_mask = smoothstep(0., INTRO_END, t);
    float smooth_outro_mask = 1.-smoothstep(OUTRO_START, 1., t);
    return dir*d*distortion_mask*smooth_intro_mask*smooth_outro_mask;
}

void main()
{
    vec2 uv = (v_uv*2.-1.)*vec2(u_resolution.x/u_resolution.y,1.)*.5+.5;
    float t = pow(fract(u_time * SPEED), 1. / EASE);    // t is the progress through the animation
    gl_FragColor = texture(u_texture, uv + get_offset(t, u_center, uv));
}
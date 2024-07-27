#version 330 core
precision highp float;

in vec4 v_col;
in vec2 v_uv;

uniform float u_time;
uniform sampler2D u_texture;

#define OCTAVE 2.0

//float noise(int lod) {
//	float val = 0.0;
//    for (int i = 0; i < lod; i++) {
//        value += amplitude * EvaluateNoise(x * frequency, y * frequency);
//        amplitude *= persistence;
//        frequency *= lacunarity;
//    }
//    return val;
//}

void main() {
	vec4 sample = texture2D(u_texture, (v_uv+vec2(cos(u_time), sin(u_time))) / OCTAVE) / OCTAVE + texture2D(u_texture, v_uv+vec2(sin(u_time), exp(cos(u_time))));
	gl_FragColor = sample * v_col;
}
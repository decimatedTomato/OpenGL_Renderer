#version 330 core
precision highp float;

in vec4 vVertexColor;
in vec2 vUV;

#define OCTAVE 2.0

uniform float uTime;
uniform sampler2D uTexture;

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
	vec4 sample = texture2D(uTexture, (vUV+vec2(cos(uTime), sin(uTime))) / OCTAVE) / OCTAVE + texture2D(uTexture, vUV+vec2(sin(uTime), exp(cos(uTime))));
	gl_FragColor = sample * vVertexColor;
}
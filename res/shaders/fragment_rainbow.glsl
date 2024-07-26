#version 330 core
precision highp float;

in vec4 vVertexColor;
in vec2 vUV;

uniform float uTime;

void main() {
	vec3 col = vUV.x * vec3(0.5, 0, 0) + vUV.y * vec3(0, 0.5, 0.5);
	col += sin(uTime * vec3(1, 2, 4) + distance(vUV, vec2(0.5))) / 2.0;
	gl_FragColor = vec4(col, 1.0);
}
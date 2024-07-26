#version 330 core
precision highp float;

in vec4 vVertexColor;
in vec2 vUV;

void main() {
	vec3 col = vec3(vUV.xy, 0);
	gl_FragColor = vec4(col, 1.0);
}
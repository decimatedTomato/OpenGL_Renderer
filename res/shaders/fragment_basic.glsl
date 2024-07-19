#version 330 core
precision highp float;

varying vec2 vUV;
varying vec4 vVertexColor;

uniform float uTime;

void main() {
//	vec3 colors = vUV.x * vec3(0.5, 0.0, 0) + vUV.y * vec3(0, 0.5, 0.5);
//	vec3 colors = vec3(vUV.xy, 0);
//	colors += sin(uTime * vec3(1, 2, 4) + distance(vUV, vec2(0.5))) / 2.0;
	vec3 colors = vVertexColor.rgb;
	gl_FragColor = vec4(colors, 1.0);
}
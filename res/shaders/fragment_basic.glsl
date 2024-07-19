#version 330 core
precision highp float;

in vec4 vVertexColor;
in vec2 vUV;

uniform float uTime;

void main() {
//	vec3 colors = vUV.x * vec3(0.5, 0.0, 0) + vUV.y * vec3(0, 0.5, 0.5);
//	colors += sin(uTime * vec3(1, 2, 4) + distance(vUV, vec2(0.5))) / 2.0;
	vec3 colors = vec3(vUV.xy, 0) + vVertexColor.rgb;
//	vec3 colors = vVertexColor.rgb;
	gl_FragColor = vec4(colors, 1.0);
}
#version 330 core
precision highp float;

in vec4 v_col;
in vec2 v_uv;

uniform float u_time;

void main() {
	vec3 col = v_uv.x * vec3(0.5, 0, 0) + v_uv.y * vec3(0, 0.5, 0.5);
	col += sin(u_time * vec3(1, 2, 4) + distance(v_uv, vec2(0.5))) / 2.0;
	gl_FragColor = vec4(col, 1.0);
}
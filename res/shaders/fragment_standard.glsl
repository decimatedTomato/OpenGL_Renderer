#version 330 core
precision mediump float;

in vec4 v_col;
in vec2 v_uv;

void main() {
	vec3 col = vec3(v_uv.xy, 0);
	gl_FragColor = vec4(col, 1.0);
}
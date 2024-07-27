#version 330 core
precision highp float;

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec4 a_col;
layout(location = 2) in vec2 a_uv;

//uniform mat4 u_model_view_matrix;
//uniform mat4 u_projection_matrix;

out vec4 v_col;
out vec2 v_uv;

void main() {
	// Apply the camera transform
	//  vec4 viewModelPosition = uModelViewMatrix * vec4(aPosition, 1.0);
	// Position the vertex
	//  gl_Position = uProjectionMatrix * viewModelPosition;

	// Pass data to fragment shader
	v_col = a_col;
	v_uv = a_uv;
	gl_Position = vec4(a_pos, 1.0);
}
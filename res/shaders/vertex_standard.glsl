#version 330 core
precision highp float;

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aVertexColor;
layout(location = 2) in vec2 aUV;

//uniform mat4 uModelViewMatrix;
//uniform mat4 uProjectionMatrix;

out vec2 vUV;
out vec4 vVertexColor;

void main() {
	// Apply the camera transform
	//  vec4 viewModelPosition = uModelViewMatrix * vec4(aPosition, 1.0);
	// Position the vertex
	//  gl_Position = uProjectionMatrix * viewModelPosition;

	// Pass data to fragment shader
	vVertexColor = aVertexColor;
	vUV = aUV;
	gl_Position = vec4(aPosition, 1.0);
}
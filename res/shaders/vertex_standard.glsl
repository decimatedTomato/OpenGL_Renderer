#version 330 core
precision highp float;

attribute vec3 aPosition;
attribute vec4 aVertexColor;
attribute vec2 aUV;

//uniform mat4 uModelViewMatrix;
//uniform mat4 uProjectionMatrix;

varying vec2 vUV;
varying vec4 vVertexColor;

void main() {
	// Apply the camera transform
	//  vec4 viewModelPosition = uModelViewMatrix * vec4(aPosition, 1.0);
	// Position the vertex
	//  gl_Position = uProjectionMatrix * viewModelPosition;

	// Pass data to fragment shader
	vUV = aUV;
	vVertexColor = aVertexColor;
	gl_Position = vec4(aPosition, 1.0);
}
#version 330 core
precision highp float;

varying vec2 vTexCoord;
varying vec4 vVertexColor;

void main() {
  vec3 colors = vec3(1);

//  colors += sin(uTime * vec3(1. , 2., 3.) + vTexCoord.x + cos(vTexCoord.y * uTime)) / 2.;

  gl_FragColor = vec4(colors, 1.0);
}
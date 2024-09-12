#version 450
#extension GL_EXT_buffer_reference : require

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec2 outUV;

struct Vertex {
  vec3 position;
  float uv_x;
  vec3 normal;
  float uv_y;
  vec4 color;
};

// Buffer address as a uint64 handle
layout(buffer_reference, std430) readonly buffer VertexBuffer{ Vertex vertices[]; };

// Push constants block
layout( push_constant ) uniform constants {
  mat4 render_matrix;
  VertexBuffer vertexBuffer;
} PushConstants;

void main() {
  // // const array of positions for the triangle
  // const vec3 positions[3] = vec3[3](
  //   vec3(1.f, 1.f, 0.f),
  //   vec3(-1.f, 1.f, 0.f),
  //   vec3(0.f, -1.f, 0.f)
  // );

  // // const array of colors for the triangle
  // const vec3 colors[3] = vec3[3](
  //   vec3(1.f, 0.f, 0.f),
  //   vec3(0.f, 1.f, 0.f),
  //   vec3(0.f, 0.f, 1.f)
  // );

  Vertex v = PushConstants.vertexBuffer.vertices[gl_VertexIndex];

  // Output the position of each vertex
  gl_Position = PushConstants.render_matrix * vec4(v.position, 1.f);
  outColor = v.color.xyz;
  outUV.x = v.uv_x;
  outUV.y = v.uv_y;
}